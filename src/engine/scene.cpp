#include "scene.h"

Scene::Scene()
{

};

/*
Scene::Scene(Scene& b)
{

};

Scene& Scene::operator=(Scene& b)
{

};
*/

void Scene::append(Entity* entity)
{
	this->objects.push_back(entity);
};

void Scene::update()
{
	for (auto entity : this->objects)
		entity->update();
};

std::vector<Entity *> Scene::getEntities()
{
	return this->objects;
}


Scene::~Scene()
{

};