#include "gui_item.h"

using namespace b_GUI;

GUIItem::GUIItem()
{
	this->type = GUI_SHAPE;
};

GUIItem::~GUIItem()
{

};

TextureImage2D* GUIItem::getTexture()
{
	return nullptr;
};


// -- Getters
const glm::vec2& GUIItem::getPosition()
{
	return this->position;
};
const glm::vec2& GUIItem::getScaling()
{
	return this->scaling;
};
float GUIItem::getRotation()
{
	return this->rotation;
};
const glm::vec4& GUIItem::getColor()
{
	return this->color;
};
GUIItemType GUIItem::getType()
{
	return this->type;
};

// -- Setters
void GUIItem::setPosition(glm::vec2 v)
{
	this->position = v;
};
void GUIItem::setScaling(glm::vec2 v)
{
	this->scaling = v;
};
void GUIItem::setRotation(float v)
{
	this->rotation = v;
};
void GUIItem::setColor(glm::vec3 c)
{
	this->color.r = c.r;
	this->color.g = c.g;
	this->color.b = c.b;
};
void GUIItem::setAlpha(float v)
{
	this->color.a = v;
};