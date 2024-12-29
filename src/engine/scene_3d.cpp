#include "scene_3d.h"

Scene3D::Scene3D(Camera* cam)
{
	this->c_main = cam;
	this->type = SCENE_STANDART;
};

Camera* Scene3D::getCameraMain()
{
	return this->c_main;
};

void Scene3D::update()
{
	this->c_main->update();
	for (Entity* entity : this->objects)
		entity->update();
};