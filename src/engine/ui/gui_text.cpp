#include "gui_text.h"

using namespace b_GUI;

GUIText::GUIText (std::string text)
{
	this->text = text;
	this->type = GUI_TEXT;
};

GUIText::~GUIText()
{

};