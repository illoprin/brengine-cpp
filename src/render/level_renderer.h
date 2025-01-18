#pragma once

#include <render/renderer.h>
#include <render/framebuffer.h>
#include <level/level.h>
#include <core/camera.h>
#include <texture/texture_storage.h>
#include <core/default_assets.h>


class LevelRenderer : public Renderer
{
public:
	LevelRenderer(Framebuffer*);

	void Render(b_Level::Level*, Camera*);
private:
	Framebuffer* fb_scene;
};