#include "gui_image.h"

using namespace b_GUI;
using namespace b_Texture;

GUIImage::GUIImage()
{
	this->type = GUI_IMAGE;
};

GUIImage::GUIImage(Texture* t)
{
	this->texture = t;
	this->type = GUI_IMAGE;
};

Texture* GUIImage::getTexture() const
{
	return this->texture;
};

void GUIImage::setTexture(Texture* t)
{
	this->texture = t;
};