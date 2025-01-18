#pragma once

#include "../util/deps.h"
#include "renderer.h"
#include "framebuffer.h"


/*
	Renderer Program Types:
		2D Rendering:
			FlatShader - draw textured primitives with only model matrix
			UIShader - draw UI

		3D Rendering:
			StandartShader - draw 3D textured models
			SpriteShader - draw textured sprites in 3D space
			DefferedShader - shader for combine several views of scene

		Post-processing shaders:
			BloomShader - use bloom effect on rendered scene
			AOShader - use ambient occlusion effect on rendered scene
			MixerShader - combine all views on main framebuffer
			(uses color correction)
*/
class MasterRenderer : public Renderer
{
	public:
		MasterRenderer();
		~MasterRenderer();

		// Delete copy operators
		MasterRenderer(Renderer&) = delete;
		MasterRenderer& operator=(MasterRenderer&) = delete;
		MasterRenderer& operator=(const MasterRenderer&) = delete;

		Framebuffer* getSceneFramebuffer() const;
		Framebuffer* getUIFramebuffer() const;

		// Clear all framebuffers
		void Clear() override;
		
		// Blit rendered pixels on main framebuffer
		void Flush();

		void WindowSizeChangeCallback() override;

		void switchRenderMode();
	private:
		Framebuffer* fb_scene;
		Framebuffer* fb_ui;
};