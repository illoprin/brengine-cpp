#pragma once

#include "utils/deps.h"
#include "scene.h"
#include "log.h"
#include "renderer.h"
#include "clock.h"
#include "log.h"
#include "program.h"
#include "input.h"

static void errorCallback(int error, const char* desc);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Engine
{
public:
	Engine();
	~Engine();

	void render(Scene& scene);
	void update(Scene& scene);
	void prepare();
	void close();

	void setVidMode(unsigned w, unsigned h);

	GLFWwindow* getWindow() const;
	Renderer* getRenderer() const;
	Log* getLogger() const;
	Clock* getClock() const;
	Input* getIO() const;
	glm::ivec2 getVidMode();

	// Delete copy operators
	Engine(Engine&) = delete;
	Engine& operator=(Engine&) = delete;
	Engine& operator=(const Engine&) = delete;

private:
	GLFWwindow* window;
	glm::ivec2 vid_mode;

	Renderer* renderer;
	Clock* clock;
	Log* log;
	Input* input;
	// TODO: Scene/Framebuffer and other stuff
	void initWindow();
	void initIO();
};
