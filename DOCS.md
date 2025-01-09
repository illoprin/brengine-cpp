# Documentation

### Namespaces

**b_Model**

	Functions and structures related to creation of model triangles
	b_Model::ModelTriangles is std::vector<b_Model::Triangle>

**b_AssetManager**

	Gives access to prepared assets such as meshes, textures and fonts

**b_GameObject**

	Transform struct (that has position, rotation, scale vectors and model matrix)
	Entity class - stores information about entire object (mesh, texture, color, alpha, and its transform)

**b_ImageIO**

	Functions for generating and writing images

**b_Texture**

	All structures and classes related to texture object generation and usage

**b_Files**

	Functions for service engine filesystem and other file-stuff

**b_Utils**

	Some utils functions

**b_Level**

	All structures and functions related to game level

**b_Font**

	All structures and functions related to font loading system.
	Structure b_Font::Font provides you to load your own TrueType (.ttf) font

**b_GUI**

	This namespace provides access to all classes related to GUI rendering

### Engine entities

**Entity**
	
	Stores information about texture (or color), mesh, collider of entire object
	The entity is required to be stored within the Scene.

**GUIScene**

	Collection of several GUI Items.
	Could be processed by Renderer and drawed on screen

**Scene3D**

	Collection of several Entities
	Stores information about Camera from whose point of view render will be performed

### Simple usage of engine `Jan 2025 Version`

```c++
#include "engine/brengine.h"

/*
	ui_ - gui object
	e_ - entitiy instance
	s_ - scene instance
*/

struct Game {
	b_GameObject::Entity* e_white_cube;
	b_GUI::GUIImage* ui_quad;
	EditorController* g_player;
};

bool is_cube_white_rotating = false;

void key_callback(int key, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_E)
			is_cube_white_rotating = !is_cube_white_rotating;
	}
};

void update(float time, float deltaTime)
{
	Game* game = (Game*)b_Engine::GetEngineUserPointer();
	// UI Scene update
	game->ui_quad->setScaling( .5f + glm::vec2( fabs( sinf(time) ) * .3f ));
	game->ui_quad->setAlpha( fabs( cosf(time) ));
	
	// 3D Scene update
	if (is_cube_white_rotating)
		game->e_white_cube->transform.rotate(glm::vec3( deltaTime * 100.f ));

	game->g_player->update(); // Player update
};

int main()
{
	b_Engine::Init();

	// Set user key callback function
	b_Engine::getIO()->setKeyCallback(key_callback);

	// Init level d0
	b_Level::LevelData d0_lvl;
	b_Model::ModelTriangles d0_tris;
	b_Level::FromBLF("d0", d0_lvl);
	b_Model::LevelToTriangles(d0_lvl, d0_tris);
	SimpleMesh levelm("d0_level");
	levelm.initFromModel(&d0_tris);

	// Init brick texture
	b_Texture::TextureImage2D t_brick (false);
	t_brick.FromPNG("brick_1");
	t_brick.setFiltering(GL_NEAREST);

	// Init rock texture
	b_Texture::TextureImage2D t_rock{false};
	t_rock.FromPNG("brown_rock");
	t_rock.setFiltering(GL_NEAREST);

	// ---- Init UI

	b_GUI::GUIItem ui_cross;
	ui_cross.setColor(glm::vec3(1.f, 0.09f, 0.79f));
	ui_cross.setScaling(glm::vec2(.015f));
	
	b_GUI::GUIImage ui_quad;
	ui_quad.setPosition(glm::vec2(-.7, -.7));
	ui_quad.setTexture(b_AssetManager::getTextureNull());

	b_GUI::GUIScene s_ui{};
	s_ui.append(&ui_cross);
	s_ui.append(&ui_quad);

	// Get poiner to engine renderer instance
	// It is not necessary in this case
	// But here I do that to simplicity
	Renderer* rend = b_Engine::getRenderer();

	// ---- Init 3D Level
	EditorController player{};
	Scene3D s_level{player.getCamera()};

	// Level entity 
	b_GameObject::Entity e_level("Level_d0");
	e_level.setProgram(rend->getProgramStandart());
	e_level.setMesh(&levelm);
	e_level.setTexture(&t_brick);

	// White cube entity
	b_GameObject::Entity e_cube_white("Cube_white");
	e_cube_white.setProgram(rend->getProgramStandart());
	e_cube_white.setMesh(b_AssetManager::getMeshCube());
	e_cube_white.transform.setPosition(glm::vec3(4, 1, 5));
	
	// Rock cube entity
	b_GameObject::Entity e_cube_rock("Cube_rock");
	e_cube_rock.setProgram(rend->getProgramStandart());
	e_cube_rock.setMesh(b_AssetManager::getMeshCube());
	e_cube_rock.setTexture(&t_rock);
	e_cube_rock.transform.setPosition(glm::vec3(4, 1, 8));

	s_level.append(&e_cube_white);
	s_level.append(&e_cube_rock);
	s_level.append(&e_level);

	b_Engine::SetScene(&s_level);
	b_Engine::SetUI(&s_ui);
	// Means that 'update' function will be called every frame before rendering
	b_Engine::SetUserUpdateFunction(update);

	Game game;
	game.e_white_cube = &e_cube_white;
	game.ui_quad = &ui_quad;
	game.g_player = &player;

	// Means that 'Game' struct pointer will be saved in engine to get it in user functions
	b_Engine::SetEngineUserPointer(&game);

	// Starts game loop
	b_Engine::Run();

	// Free resources
	b_Engine::Release();

	// Destructors of user objects will be called automatically
	return 0;
}
```

### Loading level `(*.blf)`

```c++
// Init level example

// Create level data struct
b_Level::LevelData d0_lvl;
// Create mesh data instance
b_Model::ModelTriangles d0_tris;
// Load file data to level data empty instance
b_Level::FromBLF("d0", d0_lvl);
// Triangulate level data
b_Model::LevelToTriangles(d0_lvl, d0_tris);
// Print mesh data (not required)
b_Model::printTriangles(d0_tris);

// Init simple mesh empty instance
SimpleMesh levelm("d0_level");
// Initialize mesh from mesh data
levelm.initFromModel(&d0_tris);
```

### Load your own font

```c++
namespace b_GuiFont = b_GUI::b_Font;

b_GuiFont::Font f_user{"FontUser"}; // 1st arg - Ingame name of font
// 1st arg - name of ttf file in 'assets/fonts' path
// 2nd arg - lineheight in pixels
// 3rd arg - width of font atlas
// 4th arg - height of font atlas
// Font atlas uses to get apperance of glyph on rendering stage
// The quality of rasterization depends on the atlas size and line height variables
f_user.FromTTF("user_font", 50, 1024, 1024); // Will cache atlas from 'assets/fonts/user_font.ttf' file

// Create b_GUI:GUIText instance to use this font in game
b_GUI::GUIText ui_text{
	&f_user, "Sample Text"
};
// The size of the text object depends on your font settings
// Select the size imperially
ui_text.setScaling(glm::vec2(3, 3)); // For e.g.

b_GUI::GUIScene s_ui;
s_ui.append(&ui_text);

b_Engine::SetUI(&s_ui);
```