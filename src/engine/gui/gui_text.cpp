#include "gui_text.h"

using namespace b_GUI;

namespace b_GuiFont = b_GUI::b_Font;

static unsigned gui_text_total = 0;

GUIText::GUIText (b_GuiFont::Font* f, std::string t)
{
	std::string mesh_name
		{"GUIText_" + std::to_string(gui_text_total)};
	
	// Allocate vidmemory to string vertices
	this->mesh = new BaseMesh{mesh_name.c_str()};
	// Reserved vidmemory: 16 * 6 * 256 = 24.57kb
	GLuint lb = mesh->AddDynamicBuffer(NULL, 0, sizeof(SimpleVertex2D) * 6 * GUI_MAX_STRING_LEN);
	// Link mesh attributes
	// 0. in_position
	mesh->SetDataPointer(
		lb, 2, sizeof(SimpleVertex2D), 0);
	// 1. in_texcoord
	mesh->SetDataPointer(
		lb, 2, sizeof(SimpleVertex2D), offsetof(SimpleVertex2D, texcoord.x));
	
	this->text = t;
	this->font = f;
	this->type = GUI_TEXT;
	gui_text_total++;
	need_redraw = true;
};

void GUIText::update()
{
	if (text.length() > 0)
	{
		need_redraw = text != last_text ? true : false;
		last_text = text;
	}
};

BaseMesh* GUIText::getMesh() const
{
	return this->mesh;
};
b_Font::Font* GUIText::getFont() const
{
	return this->font;
};

GUIText::~GUIText()
{
	delete this->mesh;
};