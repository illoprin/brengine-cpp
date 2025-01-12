#pragma once

#include "../util/deps.h"
#include "game_data.h"
#include "../level/level.h"
#include "../player/editor_controller.h"
#include "../mesh/base_mesh.h"
#include "../texture/texture_storage.h"
#include "../gui/gui_scene.h"

namespace b_Game
{
	class GameHandler
	{
	public:
		GameHandler(std::string);
		~GameHandler();
		b_Level::Level* getCurrentLevel() const;
		b_GUI::GUIScene* getCurrentGUI() const;

		void installLevel(uint16_t);
		void releaseLevel();

		void update();
	private:
		// Data
		b_Game::GameData game_data;

		// Globals
		EditorController p_player;
		b_Texture::TextureStorage3D t_atlas;
		b_Texture::TextureStorage3D t_palette;

		// Current loaded level
		std::unique_ptr<b_Level::Level> level_up;
		// Current loaded gui
		std::unique_ptr<b_GUI::GUIScene> gui_up;

		GameState game_state = GameState::GS_MAIN_MENU;

		void build_texture_storage();
		void build_palette();
	};
}