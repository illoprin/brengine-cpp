#pragma once

#include "../utils/deps.h"
#include "../scene_3d.h"
#include "../camera.h"
#include "program.h"
#include "texture.h"
#include "framebuffer.h"
#include "../ui/gui_scene.h"
#include "../ui/gui_item.h"
#include "../ui/gui_text_meshgen.h"
#include "../assets.h"

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

class Renderer
{
public:
	Renderer();
	~Renderer();

	friend class b_GUI::GUIScene;

	// Delete copy operators
	Renderer(Renderer&) = delete;
	Renderer& operator=(Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	
	// Clear main framebuffer
	void ctxClear();
	// Update matrices
	void Update();
	// Clear all framebuffers
	void ClearCanvas();
	// Render scene
	void RenderScene(Scene3D& scene);
	// Render GUI
	void RenderUI(b_GUI::GUIScene& scene);
	// Blit rendered pixels on screen
	void Flush();

	void FramebufferSizeChange();

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
	void ctxPrepare();
	void ctxBind();

	void init_programs();
	void init_framebuffers();

	void render_3d(Scene3D& scene);

	void render_gui_item(b_GUI::GUIItem*);
	void render_3d_entity(b_GameObject::Entity*, Program*, Camera*);
	
	RenderMode r_mode;

	// General matrices for projection
	glm::mat4 m_ortho{1.f};
	glm::mat4 m_perspective{1.f};

	// Shader Programs
	Program* p_flat;
	Program* p_standart;
	Program* p_mixer;

	// Framebuffers
	Framebuffer* fb_scene;
	Framebuffer* fb_ui;
};