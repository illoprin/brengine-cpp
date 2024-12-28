#pragma once

#include "deps.h"
#include "scene.h"

class Renderer
{
public:
	Renderer(Log* logger, GLFWwindow* win);

	// Delete copy operators
	Renderer(Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	void start();
	void end();
	void render(Scene& scene);

	~Renderer();
private:
	GLFWwindow* window;
	Log* log;
	void ctxPrepare();
};