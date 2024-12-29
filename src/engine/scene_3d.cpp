#include "scene_3d.h"

Scene3D::Scene3D(Engine* eng, Camera* cam)
{
	this->c_main = cam; this->engine = eng;
	this->type = SCENE_STANDART;
};

Camera* Scene3D::getCameraMain()
{
	return this->c_main;
};

void Scene3D::update()
{
	this->c_main->update(this->engine->getVidMode());
	for (Entity* entity : this->objects)
		entity->update();
};