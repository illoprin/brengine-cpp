#include "entity.h"

Entity::~Entity()
{

};

void Entity::update()
{
	this->update_model();
};

void Entity::update_model()
{

};

void Entity::setPosition(glm::vec3 delta)
{

};

void Entity::setRotation(glm::vec3 delta)
{

};

void Entity::setScale(glm::vec3 delta)
{

};

void Entity::move(glm::vec3 delta)
{

};

void Entity::rotate(glm::vec3 delta)
{

};

void Entity::resize(glm::vec3 delta)
{

};

BaseMesh* Entity::getMesh() const
{
	return this->mesh;
}

Program* Entity::getProgram() const
{
	return this->program;
}
