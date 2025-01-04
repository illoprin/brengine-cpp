#include "scene_3d.h"

using namespace b_GameObject;

Scene3D::Scene3D(Camera* cam)
{
	this->c_main = cam;
};

Camera* Scene3D::getCameraMain()
{
	return this->c_main;
};

void Scene3D::append(Entity* entity)
{
	this->objects.push_back(entity);
};

const std::vector<Entity *>& Scene3D::getEntities() const
{
	return this->objects;
};

void Scene3D::update(glm::ivec2& vid_mode)
{
	this->c_main->update(vid_mode);
	for (Entity* entity : this->objects)
		entity->update();
};