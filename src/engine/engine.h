#pragma once

#include "deps.h"
#include "scene.h"
#include "log.h"
#include "renderer.h"
#include "clock.h"
#include "log.h"
#include "program.h"

static void errorCallback(int error, const char* desc);
static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Engine
{
public:
	void render(Scene& scene);
	void update(Scene& scene);
	void prepare();
	void close();

	Engine();

	GLFWwindow* getWindow() const;
	Log* getLogger() const;
	Program* getDefaultProgram() const;

	// Delete copy operators
	Engine(Engine&) = delete;
	Engine& operator=(Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
	
	~Engine();
private:
	GLFWwindow* window;
	Renderer* renderer;
	Clock* clock;
	Log* log;
	Program* p_default;
	// TODO: Scene/Framebuffer and other stuff
	void initWindow();
};
