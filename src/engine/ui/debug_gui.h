#pragma once

#include "../utils/deps.h"
#include "gui_font.h"
#include "gui_text.h"
#include "gui_scene.h"
#include "../assets.h"
#include "../camera.h"

namespace b_GUI
{
	struct DebugGUI
	{
		GUIText ui_fps_text {
			b_AssetManager::getDefaultMonoFont(),
			"FPS: "
		};
		GUIText ui_info_text {
			b_AssetManager::getDefaultSansFont(),
			"Brengine 3D Alpha"
		};
		GUIText ui_player_text {
			b_AssetManager::getDefaultMonoFont(),
			"Position:"
		};
		GUIScene s_ui_debug;
		std::string str_player;
		std::string str_fps;

		DebugGUI();
		void updateFPSText();
		void updatePlayerPosText(Camera*);
	};
}
