#pragma once

#include "../util/deps.h"
#include "../util/utils.h"

namespace b_Game
{
	struct LevelAppearanceData
	{
		uint16_t itexture; // Index of texture
		uint16_t icolor; // Index of color
		bool glowing;
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
		std::map<uint16_t, std::string> textures;
		// All pathes to 3d models (*.obj or *.fbx)
		std::map<uint16_t, std::string> models;
		// Colors in palletes
		std::map<uint16_t, glm::vec3> palletes;
		// Level flat surfaces types
		std::map<uint16_t, LevelAppearanceData> lvl_appearance;

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