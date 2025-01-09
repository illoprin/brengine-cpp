#include "debug_gui.h"

#include "../engine.h"
#include "../mesh/base_mesh.h" // Stores information about total number of draw calls

using namespace b_GUI;

DebugGUI::DebugGUI()
{
	ui_fps_text.setPosition(glm::vec2(.6, .9));
	ui_fps_text.setScaling(glm::vec2(2.2, 2.2));
	ui_info_text.setPosition(glm::vec2(-.9, .9));
	ui_info_text.setScaling(glm::vec2(6.7, 6.7));
	ui_player_text.setPosition(glm::vec2(-.9, .73));
	ui_player_text.setScaling(glm::vec2(2.2, 2.2));

	s_ui_debug.append(&ui_fps_text);
	s_ui_debug.append(&ui_info_text);
	s_ui_debug.append(&ui_player_text);
};

void DebugGUI::updateInfo(
	Camera* cam
)
{
	const glm::vec3& player_pos = cam->getPosition();
	const glm::vec3& player_dir = cam->getForward();
	const float& p_pitch = cam->getPitch();
	const float& p_yaw = cam->getYaw();
	ui_player_text.text = {"Position - X: " 
		+ std::to_string(player_pos.x)
		+ " Y: " + std::to_string(player_pos.x)
		+ " Z: " + std::to_string(player_pos.x)
		+ "\nDirection - X: "
		+ std::to_string(player_dir.x)
		+ " Y: " + std::to_string(player_dir.y)
		+ " Z: " + std::to_string(player_dir.z)
		+ "\nPitch: " + std::to_string((int)p_pitch)
		+ "\nYaw: " + std::to_string((int)p_yaw)
		+ "\n\nDraw calls: " + std::to_string(b_draw_calls)
		+ "\nVertices: " + std::to_string(b_total_vertices)
	};
};

void DebugGUI::updateFPSText()
{
	float deltaTime = b_Engine::getClock()->getDeltaTime();
	int framerate = (int)(1.0 / deltaTime);
	ui_fps_text.text = {
		"FPS: " + std::to_string(framerate)
	};
	glm::vec3 fps_color;
	if (framerate >= 59)
		fps_color = {.172f, .745f, .03f}; // Green color
	else if (framerate >= 30)
		fps_color = {.93f, .839f, .109f}; // Yellow color
	else
		fps_color = {.93f, .07f, .12f}; // Red color
	ui_fps_text.setColor(fps_color);

}