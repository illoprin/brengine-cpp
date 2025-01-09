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

	game->g_player->update();
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
	TextureImage2D t_brick (false);
	t_brick.FromPNG("brick_1");
	t_brick.setFiltering(GL_NEAREST);

	// Init rock texture
	TextureImage2D t_rock{false};
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
	b_Engine::SetUserUpdateFunction(update);

	Game game;
	game.e_white_cube = &e_cube_white;
	game.ui_quad = &ui_quad;
	game.g_player = &player;

	b_Engine::SetEngineUserPointer(&game);

	b_Engine::Run();

	b_Engine::Release();
	return 0;
}