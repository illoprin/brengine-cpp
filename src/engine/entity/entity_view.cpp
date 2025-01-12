#include "entity_view.h"

using namespace b_GameObject;

// -- Getters
b_Texture::Texture* EntityAppearance::getTexture() const
{
	return this->texture;
};
const glm::vec4& EntityAppearance::getColor() const
{
	return this->color;
};
bool EntityAppearance::getFaceCullingUsing() const
{
	return this->use_face_culling;
};
float EntityAppearance::getUVScaling() const
{
	return this->uv_scaling;
};


// -- Setters
void EntityAppearance::setTexture(b_Texture::Texture* v)
{
	this->texture = v;
};
void EntityAppearance::setUseFaceCulling(bool v)
{
	use_face_culling = v;
};
void EntityAppearance::setUVScaling(float v)
{
	uv_scaling = v;
};
void EntityAppearance::setAlpha(float v)
{
	color.a = v;
};
void EntityAppearance::setColor(glm::vec3 v)
{
	color.r = v.r;
	color.g = v.g;
	color.b = v.b;
};


// -- Bools
bool EntityAppearance::hasTexture() const
{
	return texture != nullptr;
};