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


	// Init level d0
	b_Level::LevelData d0_lvl;
	b_Model::ModelTriangles d0_tris;
	b_Level::FromBLF("d0", d0_lvl);
	b_Model::LevelToTriangles(d0_lvl, d0_tris);
	SimpleMesh levelm(engine.getLogger(), "d0_level");
	levelm.initFromModel(&d0_tris);

	// Init Cube mesh from file
	std::string m_name; b_Model::ModelTriangles cube_tris; 
	b_Model::loadFromOBJ("cube", m_name, cube_tris);
	SimpleMesh m_cube{engine.getLogger(), m_name.c_str()};
	m_cube.initFromModel(&cube_tris);


	// Init engine label texture
	TextureImage2D t_engine_label{engine.getLogger(), false};
	t_engine_label.FromFile("ui_label");

	// Init null texture
	TextureImage2D t_null{engine.getLogger(), false};
	t_null.FromFile("NULL");

	// Init brick texture
	TextureImage2D t_brick (engine.getLogger(), false);
	t_brick.FromFile("brick_1");
	t_brick.setFiltering(GL_NEAREST);

	// Init rock texture
	TextureImage2D t_rock{engine.getLogger(), false};
	t_rock.FromFile("brown_rock");
	t_rock.setFiltering(GL_NEAREST);


	// ---- Init UI

	// Init label entity
	b_GUI::GUIImage ui_label{};
	ui_label.setPosition(glm::vec2(-.7f, .7f));
	ui_label.setTexture(&t_engine_label);

	b_GUI::GUIItem ui_cross{};
	ui_cross.setColor(glm::vec3(1.f, 0.09f, 0.79f));
	ui_cross.setScaling(glm::vec2(.015f));
	
	b_GUI::GUIImage ui_quad{};
	ui_quad.setPosition(glm::vec2(-.7, -.7));
	ui_quad.setTexture(&t_null);

	b_GUI::GUIScene s_ui{};
	s_ui.append(&ui_label);
	s_ui.append(&ui_cross);
	s_ui.append(&ui_quad);

	// ---- Init 3D Level
	
	EditorController player{&engine};
	Scene3D s_level{player.getCamera()};

	// Level entity 
	b_GameObject::Entity e_level("Level_d0");
	e_level.setProgram(rend->getProgramStandart());
	e_level.setMesh(&levelm);
	e_level.setTexture(&t_brick);

	// White cube entity
	b_GameObject::Entity e_cube_white("Cube_white");
	e_cube_white.setProgram(rend->getProgramStandart());
	e_cube_white.setMesh(&m_cube);
	e_cube_white.transform.setPosition(glm::vec3(4, 1, 5));
	
	// Rock cube entity
	b_GameObject::Entity e_cube_rock("Cube_rock");
	e_cube_rock.setProgram(rend->getProgramStandart());
	e_cube_rock.setMesh(&m_cube);
	e_cube_rock.setTexture(&t_rock);
	e_cube_rock.transform.setPosition(glm::vec3(4, 1, 8));

	s_level.append(&e_cube_white);
	s_level.append(&e_cube_rock);
	s_level.append(&e_level);

	while (!glfwWindowShouldClose(engine.getWindow()))
	{
		engine.OpenGameLoop();
		float time = (float) engine.getClock()->getTime();
		float deltaTime = (float) engine.getClock()->getDeltaTime();
		
		// UI Scene update
		ui_quad.setScaling( .5f + glm::vec2( fabs( sinf(time) ) * .3f ));
		ui_quad.setAlpha( fabs( cosf(time) ));

		// 3D Scene update
		e_cube_white.transform.rotate(glm::vec3( deltaTime * 100.f ));

		// Update player's position and rotation
		player.update();

		// Rendering
		rend->ClearCanvas();
		rend->RenderScene(s_level);
		rend->RenderUI(s_ui);
		rend->Flush();

		engine.CloseGameLoop();
	}

	return 0;
}