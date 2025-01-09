#include "gui_image.h"

using namespace b_GUI;
using namespace b_Texture;

GUIImage::GUIImage()
{
	this->type = GUI_IMAGE;
};

GUIImage::GUIImage (TextureImage2D* t)
{
	this->texture = t;
	this->type = GUI_IMAGE;
};

TextureImage2D* GUIImage::getTexture() const
{
	return this->texture;
};

void GUIImage::setTexture(TextureImage2D* t)
{
	this->texture = t;
};