#ifndef _B_ENGINE_H_
#define _B_ENGINE_H_

#include "utils/deps.h"
#include "log.h"
#include "clock.h"
#include "input.h"

#include "render/renderer.h"
#include "render/program.h"
#include "assets.h"

class Engine
{
public:
	Engine();
	~Engine();

	void OpenGameLoop();
	void CloseGameLoop();

	void setVidMode(unsigned w, unsigned h);
	// Toggle cursor
	void toggleGameMode();
	// Save main framebuffer pixel data to image
	void takeScreenshot();

	GLFWwindow* getWindow() const;
	Renderer* getRenderer() const;
	Log* getLogger() const;
	Clock* getClock() const;
	Input* getIO() const;
	bool isGameMode() const;
	glm::ivec2 getVidMode();

	// Delete copy operators
	Engine(Engine&) = delete;
	Engine(const Engine&) = delete;
	Engine& operator=(Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

private:
	GLFWwindow* window;
	glm::ivec2 vid_mode;

	Renderer* renderer;
	Clock* clock;
	Log* log;
	Input* input;

	void init_window();
	void init_io();

	/* TODO: Scene/Framebuffer and other stuff */

	bool is_game_mode = false;
};
#endif