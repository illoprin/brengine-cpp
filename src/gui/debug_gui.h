#pragma once

#include "../util/deps.h"
#include "gui_font.h"
#include "gui_text.h"
#include "gui_scene.h"
#include "../core/default_assets.h"
#include "../core/camera.h"

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
		void updateInfo(Camera*);
	};
}
