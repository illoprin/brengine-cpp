#pragma once

#include "renderer.h"
#include "framebuffer.h"
#include "../level/level.h"
#include "../core/camera.h"
#include "../texture/texture_storage.h"

class LevelRenderer : public Renderer
{
public:
	LevelRenderer(Framebuffer*);

	void Render(b_Level::Level*, Camera*);
private:
	Framebuffer* fb_scene;
};