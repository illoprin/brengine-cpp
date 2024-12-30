# Documentation

### Namespaces

**b_Model**

	Functions and structures related to creation of model triangles
	b_Model::ModelTriangles is std::vector<b_Model::Triangle>

**b_Assets**

	Gives access to prepared assets such as meshes, textures and etc

**b_ImageIO**

	Functions for generating and writing images

**b_Files**

	Functions for service engine filesystem and other file-stuff

**b_Utils**

	Some utils functions

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

### Simple usage of engine `Dec 2024 Version`

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

	// Init gradient texture
	// Generate bytes
	std::vector<unsigned char> gradient_b(64 * 64 * 3);
	b_ImageIO::GenBytes(gradient_b, 64, 64);
	// Sent bytes to texture image
	TextureImage2D t_grad (engine.getLogger(), false);
	t_grad.FromBytes(64, 64, 3, GL_RGB, gradient_b);
	t_grad.setFiltering(GL_LINEAR);
	t_grad.setWrapping(GL_CLAMP_TO_EDGE);

	// Init UI

	// Init label entity
	// Entity constructor (Mesh*, Program*, position, rotation, scale)
	Entity label{ 
		&qm, rend->getProgramFlat(), glm::vec3(-.7f, .8f, 0.f), glm::vec3(0.f), glm::vec3(.5f) };
	label.setTexture(&t_engine_label);

	Entity crosshair{ &qm, rend->getProgramFlat() };
	crosshair.setScale(glm::vec3(.01f));
	
	Entity test_quad{ &qm, rend->getProgramFlat() };
	test_quad.setPosition(glm::vec3(-.7, -.7, 0));
	test_quad.setScale(glm::vec3(.33f));
	test_quad.setTexture(&t_null);

	Scene ui_scene;
	ui_scene.append(&label);
	ui_scene.append(&crosshair);
	ui_scene.append(&test_quad);


	EditorController player{&engine};
	Scene3D test_level_3d{&engine, player.getCamera()};

	Entity l_cube{&cubem, rend->getProgramStandart()};
	l_cube.setTexture(&t_brick);

	Entity l_quad_1{&qm, rend->getProgramStandart()};
	l_quad_1.setTexture(&t_grad);
	l_quad_1.setPosition(glm::vec3(-3, 1, -1));
	
	test_level_3d.append(&l_cube);
	test_level_3d.append(&l_quad_1);

	while (!glfwWindowShouldClose(engine.getWindow()))
	{
		engine.prepare();
		
		// UI Scene update
		float time = (float) engine.getClock()->getTime();
		test_quad.setScale(glm::vec3( .2f + fabs( sinf(time) ) * .13f ));
		test_quad.setAlpha(fabs( cosf(time) ));
		engine.update(ui_scene);

		// 3D Scene update
		if (quad_rot_active)
			l_cube.rotate(glm::vec3(engine.getClock()->getDeltaTime() * 100.f));
		
		player.update();
		engine.update(test_level_3d);
		
		// Rendering
		engine.render_start();
		engine.render(test_level_3d);
		engine.render(ui_scene);

		engine.close();
	}

	return 0;
}
```