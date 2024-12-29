#include "scene.h"

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

std::vector<Entity *> Scene::getEntities() const
{
	return this->objects;
};

SceneTypes Scene::getType()
{
	return this->type;
};

Camera* Scene::getCameraMain()
{
	return nullptr;
};


Scene::~Scene()
{

};