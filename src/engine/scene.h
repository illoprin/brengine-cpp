#pragma once

#include "utils/deps.h"

#include "game_object/entity.h"
#include "camera.h"

using namespace b_GameObject;

class Scene
{
public:
	Scene() : type(SCENE_FLAT) {};
	~Scene();

	// Delete copy constructors
	Scene& operator=(Scene&) = delete;
	Scene(Scene& b) = delete;
	Scene(const Scene& b) = delete;

	virtual void update();
	void append(Entity* entity);

	std::vector<Entity *> getEntities() const;
	SceneTypes getType();

	virtual Camera* getCameraMain();
protected:
	std::vector<Entity *> objects;
	SceneTypes type;
};