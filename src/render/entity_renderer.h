#pragma once

#include "program.h"
#include "renderer.h"
#include "framebuffer.h"
#include "../core/camera.h"
#include "../entity/entity_view.h"
#include "../texture/texture.h"
#include "../entity/entity.h"
#include "../core/context.h"


class EntityRenderer : public Renderer
{
public:
	EntityRenderer(Framebuffer* scene);

	void Render(const std::vector<b_GameObject::Entity *>&, Camera*);
private:
	Framebuffer* fb_scene;	
};