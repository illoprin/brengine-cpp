#pragma once

#include "utils/deps.h"
#include "scene.h"
#include "camera.h"
#include "program.h"
#include "texture.h"
#include "framebuffer.h"

/*
	Renderer Program Types:
		2D Rendering:
			FlatShader - draw textured primitives with only model matrix
			UIShader - draw UI

		3D Rendering:
			StandertShader - draw 3D textured models
			SpriteShader - draw textured sprites in 3D space
			DefferedShader - shader for combine several views of scene

		Post-processing shaders:
			BloomShader - use bloom effect on rendered scene
			AOShader - use ambient occlusion effect on rendered scene
			MixerShader - combine all views on main framebuffer
			(uses color correction)
*/

class Renderer
{
public:
	Renderer(Log* logger, GLFWwindow* win);
	~Renderer();

	// Delete copy operators
	Renderer(Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	
	// Clear main framebuffer
	void ctxClear();
	// Clear all framebuffers
	void ClearCanvas();
	// Render scene
	void RenderScene(Scene& scene);
	// Blit rendered pixels on screen
	void Flush();

	void setRenderMode(RenderMode mode);
	void switchRenderMode();

	void ctxEnableFaceCulling();
	void ctxDisableFaceCulling();

	void ctxEnableDepthTest();
	void ctxDisableDepthTest();

	// Use this to render 2D textured/colored stuff
	Program* getProgramFlat() const;
	// Use this to render 3D textured/colored stuff
	Program* getProgramStandart() const;
	// Use this to render screenquad
	Program* getProgramCanvas() const;
private:
	GLFWwindow* window;
	Log* log;
	
	void ctxPrepare();
	void ctxBind();

	void init_programs();
	void init_framebuffers();
	void init_help_meshes();

	void render_flat(Scene& scene);
	void render_3d(Scene& scene);

	void render_flat_entity(Entity*, Program*, int, int);
	void render_3d_entity(Entity*, Program*, Camera*);
	
	RenderMode r_mode;

	// Shader Programs
	Program* p_flat;
	Program* p_standart;
	Program* p_mixer;

	// Screen quad
	BaseMesh* m_squad;
	// Framebuffers
	Framebuffer* fb_scene;
	Framebuffer* fb_ui;
};