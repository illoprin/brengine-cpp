# Documentation

### Namespaces

**b_Model**

	Functions and structures related to creation of model triangles
	b_Model::ModelTriangles is std::vector<b_Model::Triangle>

**b_Assets**

	Gives access to prepared assets such as meshes, textures and etc

**b_GameObject**

	Transform struct (that has position, rotation, scale vectors and model matrix)
	Entity class - stores information about entire object (mesh, texture, color, alpha, and its transform)

**b_ImageIO**

	Functions for generating and writing images

**b_Files**

	Functions for service engine filesystem and other file-stuff

**b_Utils**

	Some utils functions

**b_Level**

	All structures and functions related to game level

### Engine entities

**Entity**
	
	Stores information about texture (or color), mesh, collider of entire object
	The entity is required to be stored within the Scene.

**Scene**

	Collection of several entities.
	Could be processed by Renderer and drawed on screen

**Scene3D**

	The same Scene.
	Unlike the usual one, stores information about Camera from whose point of view render will be performed

### Simple usage of engine `Jan 2025 Version`

```c++
#include "engine/brengine.h"

Engine engine;

bool quad_rot_active = false;

void key_callback(int key, int action, int mods)
{
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		quad_rot_active = !quad_rot_active;
	}
	printf("\nKey: %d\nAction: %d\n", key, action);
}

int main()
{
	// Get pointer to renderer instance
	// It is not necessary to do this
	// In this code it is done for simplicity
	Renderer* rend = engine.getRenderer();
	// Set user key callback function
	engine.getIO()->setKeyCallback(key_callback);
	// Init triangles of quad
	b_Model::ModelTriangles quad_tris = b_AssetManager::QuadTriangles(.5f);

	// Init triangles of cube.obj
	b_Model::ModelTriangles cube_tris;
	std::string cube_obj_name;
	b_Model::loadFromOBJ("cube", cube_obj_name, cube_tris);

	// Init mesh for quads
	SimpleMesh qm(engine.getLogger(), "quad");
	qm.initFromModel(&quad_tris);

	// Init mesh for cubes
	SimpleMesh cubem(engine.getLogger(), cube_obj_name.c_str());
	cubem.initFromModel(&cube_tris);

	// Init engine label texture
	TextureImage2D t_engine_label{engine.getLogger(), false};
	t_engine_label.FromFile("textures/ui_label");

	// Init null texture
	TextureImage2D t_null{engine.getLogger(), false};
	t_null.FromFile("textures/NULL");

	// Init brick texture
	TextureImage2D t_brick (engine.getLogger(), false);
	t_brick.FromFile("textures/brick_1");
	t_brick.setFiltering(GL_NEAREST);

	// Init brown rock texture
	TextureImage2D t_rock (engine.getLogger(), false);
	t_rock.FromFile("textures/brown_rock");
	t_rock.setFiltering(GL_NEAREST);

	// Init brown rock texture
	TextureImage2D t_gray_rock (engine.getLogger(), false);
	t_gray_rock.FromFile("textures/gray_rock");
	t_gray_rock.setFiltering(GL_NEAREST);


	// Init gradient texture
	// Generate bytes
	std::vector<unsigned char> gradient_b(64 * 64 * 3);
	b_ImageIO::GenBytes(gradient_b, 64, 64);
	// Sent bytes to texture image
	TextureImage2D t_grad (engine.getLogger(), false);
	t_grad.FromBytes(64, 64, 3, GL_RGB, gradient_b);
	t_grad.setFiltering(GL_LINEAR);
	t_grad.setWrapping(GL_CLAMP_TO_EDGE);

	// ---- Init UI

	// Init label entity
	// Entity constructor (std::string name, b_GameObject::Transform t)
	b_GameObject::Entity label{ "UI_label" };
	label.setMesh(&qm);
	label.setProgram(rend->getProgramFlat());
	label.transform.setPosition(glm::vec3(-.7f, .8f, 0.f));
	label.transform.setScale(glm::vec3(.5f));
	label.setTexture(&t_engine_label);

	b_GameObject::Entity crosshair{ "UI_crosshair" };
	crosshair.setMesh(&qm);
	crosshair.setProgram(rend->getProgramFlat());
	crosshair.transform.setScale(glm::vec3(.01f));
	
	b_GameObject::Entity test_quad{ "UI_test_quad" };
	test_quad.setMesh(&qm);
	test_quad.setProgram(rend->getProgramFlat());
	test_quad.transform.setPosition(glm::vec3(-.7, -.7, 0));
	test_quad.transform.setScale(glm::vec3(.33f));
	test_quad.setTexture(&t_null);

	Scene ui_scene;
	ui_scene.append(&label);
	ui_scene.append(&crosshair);
	ui_scene.append(&test_quad);

	// ---- Init 3D Level
	EditorController player{&engine};
	Scene3D test_level_3d{&engine, player.getCamera()};

	b_GameObject::Entity l_cube{"3d_cube"};
	l_cube.setMesh(&cubem);
	l_cube.setProgram(rend->getProgramStandart());
	l_cube.setTexture(&t_brick);

	b_GameObject::Entity l_cube_1{"3d_cube"};
	l_cube_1.setMesh(&cubem);
	l_cube_1.setProgram(rend->getProgramStandart());
	l_cube_1.setTexture(&t_rock);
	l_cube_1.transform.setPosition(glm::vec3(-1.5, 0, 2));

	b_GameObject::Entity l_quad_1{ "3d_quad" };
	l_quad_1.setMesh(&qm);
	l_quad_1.setProgram(rend->getProgramStandart());
	l_quad_1.transform.setPosition(glm::vec3(-3, .5f, -1));
	l_quad_1.setTexture(&t_grad);

	b_GameObject::Entity l_ground{ "3d_ground" };
	l_ground.setMesh(&qm);
	l_ground.setProgram(rend->getProgramStandart());
	l_ground.transform.setScale(glm::vec3(10.f));
	l_ground.transform.setRotation(glm::vec3(90.f, 0.f, 0.f));
	l_ground.setTexture(&t_gray_rock);
	l_ground.setUVScaling(10.f);
	
	test_level_3d.append(&l_cube);
	test_level_3d.append(&l_quad_1);
	test_level_3d.append(&l_ground);
	test_level_3d.append(&l_cube_1);

	std::vector<Scene *> scenes_to_render{&ui_scene, &test_level_3d};

	while (!glfwWindowShouldClose(engine.getWindow()))
	{
		engine.OpenGameLoop();
		
		// UI Scene update
		float time = (float) engine.getClock()->getTime();
		test_quad.transform.setScale(
			glm::vec3( .2f + fabs( sinf(time) ) * .13f ));
		test_quad.setAlpha(fabs( cosf(time) ));

		// 3D Scene update
		if (quad_rot_active)
			l_cube.transform.rotate(
				glm::vec3(engine.getClock()->getDeltaTime() * 100.f));
		l_quad_1.setColor(glm::vec3( 1.f + sinf(time), 1.f + cosf(time), 1.f + sinf(time) ));

		// Update player's position and rotation
		player.update();

		// Updates scenes
		ui_scene.update(); 
		test_level_3d.update();
		
		// Rendering
		rend->ClearCanvas();
		rend->RenderScene(test_level_3d);
		rend->RenderScene(ui_scene);
		rend->Flush();

		engine.CloseGameLoop();
	}

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
SimpleMesh levelm(engine.getLogger(), "d0_level");
// Initialize mesh from mesh data
levelm.initFromModel(&d0_tris);
```