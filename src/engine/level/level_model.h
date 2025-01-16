#pragma once

#include "../util/deps.h"
#include "../game/game_data.h"
#include "level_data.h"


namespace b_Level
{
	struct LevelVertex // 44 bytes
	{
		glm::vec3 position; // 12 bytes
		glm::vec2 texcoord; // 8 bytes
		glm::vec3 normal; // 12 bytes
		int textureId; // 2 bytes
		int textureType; // From enum FlatTypes
		float glow_intensity; // 4 bytes
	};

	void printLevelVertices(const std::vector<LevelVertex>&); 

	// BL - Brengine level
	enum FlatType
	{
		BL_TEXTURE_NULL = 0,
		BL_TEXTURE_COLOR, // Use color only
		BL_TEXTURE_FLAT,  // Use flat texture
		BL_GLOWING,
		BL_NUM_FLATTYPES
	};

	void LevelDataToVertices(
		b_Level::LevelData&, b_Game::GameData&, std::vector<LevelVertex>&
	);
};