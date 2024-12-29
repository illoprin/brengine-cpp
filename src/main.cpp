
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
	Renderer* rend = engine.getRenderer();
	engine.getIO()->setKeyCallback(key_callback);

	ModelTriangles quad_tris = b_AssetManager::QuadTriangles(.5f);

	SimpleMesh qm(engine.getLogger(), "quad");
	qm.initFromModel(&quad_tris);

	TextureImage2D t_engine_label{engine.getLogger(), false};
	t_engine_label.FromFile("textures/ui_label");

	TextureImage2D t_null{engine.getLogger(), false};
	t_null.FromFile("textures/NULL");

	TextureImage2D t_brick (engine.getLogger(), false);
	t_brick.FromFile("textures/brick_1");
	t_brick.setFiltering(GL_NEAREST);

	std::vector<unsigned char> gradient_b(64 * 64 * 3);
	b_ImageIO::GenBytes(gradient_b, 64, 64);
	TextureImage2D t_grad (engine.getLogger(), false);
	t_grad.FromBytes(64, 64, 3, GL_RGB, gradient_b);
	t_grad.setFiltering(GL_LINEAR);

	// Init UI
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
	Scene3D test_level{&engine, player.getCamera()};

	Entity l_quad{&qm, rend->getProgramStandart()};
	l_quad.setTexture(&t_brick);

	Entity l_quad_1{&qm, rend->getProgramStandart()};
	l_quad_1.setTexture(&t_grad);
	l_quad_1.setPosition(glm::vec3(-3, 1, -1));
	
	test_level.append(&l_quad);
	test_level.append(&l_quad_1);

	while (!glfwWindowShouldClose(engine.getWindow()))
	{
		engine.prepare();
		
		// UI Scene update
		float time = (float) engine.getClock()->getTime();
		test_quad.setScale(glm::vec3( .2f + fabs( sinf(time) ) * .13f ));
		test_quad.setAlpha(fabs( cosf(time) ));
		engine.update(ui_scene);

		// 3D Scene update
		if (quad_rot_active) l_quad.rotate(glm::vec3(engine.getClock()->getDeltaTime() * 100.f));
		
		player.update();
		engine.update(test_level);
		
		// Rendering
		engine.render_start();
		engine.render(test_level);
		engine.render(ui_scene);

		engine.close();
	}

	return 0;
}