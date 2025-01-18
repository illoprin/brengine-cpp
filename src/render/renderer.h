#pragma once

#include <render/program.h>


// Render
enum RenderMode
{
	RENDER_TEXTURED = 0,
	RENDER_NORMAL,
	RENDER_WIRE,
	RENDER_DEPTH,
};

extern RenderMode b_render_mode;



class Renderer
{
public:
	Renderer();
	~Renderer();

	// Delete copy operators
	Renderer(Renderer&) = delete;
	Renderer(const Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;

	const Program& getProgram();
	virtual void Clear();
	virtual void WindowSizeChangeCallback();
protected:
	Program p_main;
};