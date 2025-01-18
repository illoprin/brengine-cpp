#pragma once

#include <render/program.h>
#include <render/renderer.h>
#include <render/framebuffer.h>
#include <core/camera.h>
#include <texture/texture.h>
#include <entity/entity.h>
#include <core/context.h>


class EntityRenderer : public Renderer
{
public:
	EntityRenderer(Framebuffer* scene);

	void Render(const std::vector<b_GameObject::Entity *>&, Camera*);
private:
	Framebuffer* fb_scene;	
};