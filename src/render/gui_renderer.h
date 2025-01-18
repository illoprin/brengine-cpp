#pragma once

#include <render/renderer.h>
#include <render/framebuffer.h>
#include <gui/gui_item.h>
#include <core/default_assets.h>
#include <gui/gui_text.h>


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