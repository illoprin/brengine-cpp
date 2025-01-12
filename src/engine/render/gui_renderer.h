#pragma once

#include "renderer.h"
#include "framebuffer.h"
#include "../gui/gui_item.h"

class GUIRenderer : public Renderer
{
public:
	GUIRenderer(Framebuffer* fb_ui);
	
	// Copy operators will be deleted automatically
	void Render(const std::vector<b_GUI::GUIItem *>&);
private:
	Framebuffer* fb_ui;

	glm::mat4 m_ortho;
};