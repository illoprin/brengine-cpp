#include "transform.h"

using namespace b_GameObject;

Transform::Transform()
{
	this->init_zero();
};

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
{
	this->position = pos;
	this->rotation = rot;
	this->scale = scl;
	this->UpdateModel();
};

void Transform::init_zero()
{
	this->position = glm::vec3(0.f);
	this->rotation = glm::vec3(0.f);
	this->scale = glm::vec3(1.f);
	this->model = glm::mat4(1.f);
};

void Transform::UpdateModel()
{
	model = glm::mat4(1.f);

	// Applying transformations order
	// 		position -> rotation -> scale
	model = glm::translate(model, this->position);
	
	model = glm::rotate(model,
		glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model,
		glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model,
		glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	
	model = glm::scale(model, this->scale);
};

Transform& Transform::operator=(Transform& b)
{
	if (this != &b)
	{
		this->position = b.getPosition();
		this->rotation = b.getRotation();
		this->scale = b.getScale();
	};
	return *this;
};

Transform::Transform(Transform& b)
{
	*this = b;
};

void Transform::move(glm::vec3 offset)
{
	position += offset;
};

void Transform::rotate(glm::vec3 angle)
{
	rotation += angle;
};

void Transform::resize(glm::vec3 factor)
{
	scale *= factor;
};

/* Setters */
void Transform::setPosition(glm::vec3 pos)
{
	position = pos;
};

void Transform::setRotation(glm::vec3 rot)
{
	rotation = rot;
};

void Transform::setScale(glm::vec3 sc)
{
	scale = sc;
};

/* Getters */
glm::vec3 Transform::getPosition() const
{
	return position;
};

glm::vec3 Transform::getRotation() const
{
	return rotation;
};

glm::vec3 Transform::getScale() const
{
	return scale;
};

glm::mat4 Transform::getModel() const
{
	return model;
};

Transform::~Transform()
{

};