#include "entity.h"

using namespace b_GameObject;

Entity::Entity(std::string n)
{
	this->name = n;

	this->init_view();
};

Entity::Entity(std::string n, Transform t)
{
	this->name = n;
	this->transform = t;

	this->init_view();
};

void Entity::init_view()
{
	this->mesh = nullptr;
	printf("Entity.%s - Created!\n", this->name.c_str());
};

bool Entity::hasView()
{
	return (this->mesh != nullptr);
};

void Entity::update()
{
	// ...
};

Entity::~Entity()
{

};

// -- Setters
void Entity::setMesh(BaseMesh* m)
{
	this->mesh = m;
};
void Entity::setAppearance(const EntityAppearance& a)
{
	this->ap = a;
}

// -- Getters
BaseMesh* Entity::getMesh() const
{
	return this->mesh;
};
const std::string& Entity::getName() const
{
	return this->name;
};
const EntityAppearance& Entity::getAppearance() const
{
	return ap;
};