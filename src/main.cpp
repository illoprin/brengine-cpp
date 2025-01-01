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

	b_GameObject::Entity l_quad_1{ "3d_quad" };
	l_quad_1.setMesh(&qm);
	l_quad_1.setProgram(rend->getProgramStandart());
	l_quad_1.transform.setPosition(glm::vec3(-3, 1, -1));
	l_quad_1.setTexture(&t_grad);
	
	test_level_3d.append(&l_cube);
	test_level_3d.append(&l_quad_1);

	std::vector<Scene *> scenes_to_render{&ui_scene, &test_level_3d};

	while (!glfwWindowShouldClose(engine.getWindow()))
	{
		engine.prepare();
		
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


		player.update();
		engine.update(ui_scene); // Now do nothing
		engine.update(test_level_3d); // Now only updates camera

		/*
			Rendering multiple scenes to fbos:
			engine.render_start();
				engine.rendering_queue(std::vector<Scene *>);
			engine.render_flush();
			engine.close_game_loop();
		*/
		
		// Rendering
		engine.render_start();
		engine.render(test_level_3d);
		engine.render(ui_scene);

		engine.close();
	}

	return 0;
}