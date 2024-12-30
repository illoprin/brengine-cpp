#pragma once

#include "utils/deps.h"
#include "scene.h"
#include "camera.h"
#include "program.h"

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
	
	void start();
	void render(Scene& scene);

	void setRenderMode(RenderMode mode);
	void switchRenderMode();

	void ctxEnableFaceCulling();
	void ctxDisableFaceCulling();

	void ctxEnableDepthTest();
	void ctxDisableDepthTest();

	Program* getProgramFlat() const;
	Program* getProgramStandart() const;
	Program* getProgramCanvas() const;

private:
	GLFWwindow* window;
	Log* log;
	
	void ctxPrepare();
	void initPrograms();

	void render_flat(Scene& scene);
	void render_3d(Scene& scene);
	
	RenderMode r_mode;

	Program* p_flat;
	Program* p_standart;
};