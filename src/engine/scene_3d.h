#pragma once

#include "scene.h"
#include "camera.h"
#include "engine.h"

/*
 *	
 * Difference from initial Scene: On update sends information about camera view and perspective
 * 
 */
class Scene3D : public Scene
{
public:
	Scene3D(Engine* eng, Camera* cam);

	void update() override;

	Camera* getCameraMain() override;
protected:
	Camera* c_main;
	Engine* engine;
};
