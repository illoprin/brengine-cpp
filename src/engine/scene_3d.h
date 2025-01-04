#pragma once

#include "camera.h"
#include "game_object/entity.h"

/*
 *	
 * Difference from initial Scene: On update sends information about camera view and perspective
 * 
 */
class Scene3D
{
public:
	Scene3D(Camera* cam);

	void update(glm::ivec2& vid_mode);
	void append(b_GameObject::Entity* entity);

	const std::vector<b_GameObject::Entity *>& getEntities() const;

	Camera* getCameraMain();
protected:
	Camera* c_main;
	std::vector<b_GameObject::Entity *> objects;
};
