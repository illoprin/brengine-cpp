#include "engine/brengine.h"

void key_callback(int key, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		
	}
}

int main()
{
	b_Engine::Init();
	b_Engine::getIO()->setKeyCallback(key_callback);
	
	b_Game::GameHandler game_handler{"initial"};
	game_handler.installLevel(0);

	b_Texture::TextureModel t_rock;
	t_rock.FromPNG("brown_rock");

	b_Texture::TextureModel t_brick;
	t_brick.FromPNG("brick_1");

	// Rock appearance
	b_GameObject::EntityAppearance a_rock;
	a_rock.setTexture(&t_rock);
	a_rock.setUVScaling(10.f);

	// Brick appearance
	b_GameObject::EntityAppearance a_brick;
	a_brick.setTexture(&t_brick);

	b_GameObject::Entity e_cube{"CubeEntity"};
	e_cube.setMesh(b_AssetManager::getMeshCube());
	e_cube.setAppearance(a_brick);
	e_cube.transform.move({0, 1, 0});

	b_GameObject::Entity e_ground{"GroundEntity"};
	e_ground.setMesh(b_AssetManager::getMeshQuad());
	e_ground.transform.setRotation({90.f, 0, 0});
	e_ground.transform.setScale({10.f, 10.f, 10.f});
	e_ground.setAppearance(a_rock);

	std::vector<b_GameObject::Entity *> l_entity{&e_cube, &e_ground};
	
	EditorController player;
	Camera* cam = player.getCamera();

	b_GUI::GUIItem ui_crosshair{};
	ui_crosshair.setScaling({.015f, .015f});

	b_GUI::GUIText ui_pay_text{b_AssetManager::getDefaultGameFont(), "PAY\n NOW"};
	ui_pay_text.setScaling({7, 7});
	ui_pay_text.setColor({ 0.92, 0.64, 0.05 });
	ui_pay_text.setPosition({-.7, -.7});

	b_GUI::GUIScene s_ui;
	s_ui.append(&ui_crosshair);
	s_ui.append(&ui_pay_text);

	MasterRenderer* r_master = b_Engine::getRenderer();
	EntityRenderer r_entity{r_master->getSceneFramebuffer()};
	LevelRenderer r_level{r_master->getSceneFramebuffer()};
	GUIRenderer r_gui{r_master->getUIFramebuffer()};

	// Game loop
	while (b_Engine::isRunning())
	{
		// Pool events, start clock
		b_Engine::OpenGameLoop();

		// Update
		b_Engine::UpdateDebugGUI(cam);
		player.update();

		// Update GUI
		ui_pay_text.setScaling(
			glm::vec2{ 5.f + 2.f * fabs(sinf( (float)b_Engine::getClock()->getTime() )) }
		);

		// Render
		r_master->Clear();

		r_level.Render(game_handler.getCurrentLevel(), cam);
		r_gui.Render(s_ui.getItems());
		b_Engine::RenderDebugGUI(r_gui);
		
		r_master->Flush();
		// Swap buffers and update clock
		b_Engine::CloseGameLoop();
	};

	b_Engine::Release();
	return 0;
}
