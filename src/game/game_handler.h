#pragma once

#include <util/deps.h>
#include <game/game_data.h>
#include <level/level.h>
#include <player/editor_controller.h>
#include <texture/texture_storage.h>

#define B_PALETTE_TILE_SIZE 10
#define B_FLAT_TEXTURE_SIZE 64

enum GameState {
	GS_NONE = 0,
	GS_MAIN_MENU = 0,
	GS_LEVEL,
	GS_PAUSED,
};

namespace b_Game
{
	class GameHandler
	{
	public:
		GameHandler(std::string);
		~GameHandler();
		b_Level::Level* getCurrentLevel() const;

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

		GameState game_state = GameState::GS_MAIN_MENU;

		void build_texture_storage();
		void build_palette();
	};
}