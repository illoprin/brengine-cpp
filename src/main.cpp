#include "engine/brengine.h"

Engine engine;

void key_callback(int key, int action, int mods)
{
	// printf("\nKey: %d\nAction: %d\n", key, action);
};

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

	// Init mesh for quads
	SimpleMesh qm(engine.getLogger(), "quad");
	qm.initFromModel(&quad_tris);

	// Init level d0
	b_Level::LevelData d0_lvl;
	b_Model::ModelTriangles d0_tris;
	b_Level::FromBLF("d0", d0_lvl);
	b_Model::LevelToTriangles(d0_lvl, d0_tris);
	SimpleMesh levelm(engine.getLogger(), "d0_level");
	levelm.initFromModel(&d0_tris);

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

	Scene s_ui;
	s_ui.append(&label);
	s_ui.append(&crosshair);
	s_ui.append(&test_quad);

	// ---- Init 3D Level
	
	EditorController player{&engine};
	Scene3D s_level{&engine, player.getCamera()};

	b_GameObject::Entity e_level("Level_d0");
	e_level.setProgram(rend->getProgramStandart());
	e_level.setMesh(&levelm);
	e_level.setTexture(&t_brick);
	s_level.append(&e_level);

	while (!glfwWindowShouldClose(engine.getWindow()))
	{
		engine.OpenGameLoop();
		
		// UI Scene update
		float time = (float) engine.getClock()->getTime();
		test_quad.transform.setScale(
			glm::vec3( .2f + fabs( sinf(time) ) * .13f ));
		test_quad.setAlpha(fabs( cosf(time) ));

		// 3D Scene update

		// Update player's position and rotation
		player.update();

		// Updates scenes
		s_ui.update(); 
		s_level.update();
		
		// Rendering
		rend->ClearCanvas();
		rend->RenderScene(s_level);
		rend->RenderScene(s_ui);
		rend->Flush();

		engine.CloseGameLoop();
	}

	return 0;
}