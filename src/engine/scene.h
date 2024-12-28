#pragma once

#include "deps.h"

#include "game_object/entity.h"

class Scene
{
public:
	Scene();
	~Scene();

	// Copy operator
	Scene& operator=(Scene& b) noexcept;
	// std::move operator
	Scene(Scene& b) noexcept;

	void update();
	void append(Entity* entity);

	std::vector<Entity *> getEntities();
private:
	std::vector<Entity *> objects;
};