#pragma once

#include "../utils/deps.h"
#include "../engine.h"
#include "game_data.h"

namespace b_Game
{
	struct GameHandler
	{
	private:
		GameData lgd; // Local game data
		
		void initTextures();
	public:
		GameHandler(std::string);
		~GameHandler();
		
		Scene3D* initLevel();
		void releaseLevel(Scene3D*);
	};
};