#pragma once

#include "../util/deps.h"
#include "program.h"

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