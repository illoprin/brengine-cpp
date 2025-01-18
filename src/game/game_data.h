#pragma once

#include <util/deps.h>
#include <util/utils.h>
#include <core/log.h>

namespace b_Game
{
	struct LevelAppearanceData
	{
		uint16_t itexture; // Index of texture
		uint16_t icolor; // Index of color
		float glow_intensity;
	};

	struct GameData
	{
		// Name of game
		std::string game_name;
		// Name of file, that descripts the game
		std::string file_name;
		// All pathes to levels
		std::vector<std::string> levels;
		// All pathes to textures (*.bmp only)
		std::vector<std::string> textures;
		// All pathes to 3d models (*.obj or *.fbx)
		std::vector<std::string> models;
		// Colors in palletes
		std::vector<glm::ivec3> pallete;
		// Level flat surfaces types
		std::vector<LevelAppearanceData> lvl_appearance;

		void print();
	};

	enum BGDSeekType
	{
		BGD_NOTHING = 0,
		BGD_ENT_APPEARANCE,
		BGD_LVL_APPEARANCE,
		BGD_PALLETE
	};

	void GameDataFromBGD(const std::string file_name, GameData& gd);
};