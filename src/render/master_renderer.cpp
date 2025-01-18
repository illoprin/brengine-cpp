#include "master_renderer.h"

#include <core/context.h>
#include <core/engine.h>

RenderMode b_render_mode = RENDER_TEXTURED;

MasterRenderer::MasterRenderer()
{
	// Use this to render screenquad and do basic post process
	p_main.InitVertexAndFragment("b_godmixer");

	fb_scene = new Framebuffer{"Scene"};
	fb_scene->bind();
	fb_scene->initColorAttachment(WIN_WIDTH, WIN_HEIGHT);
	fb_scene->initDepthAttachment(WIN_WIDTH, WIN_HEIGHT);
	fb_scene->check();
	fb_scene->unbind();

	fb_ui = new Framebuffer{"GUI"};
	fb_ui->initColorAttachment(WIN_WIDTH, WIN_HEIGHT);
	fb_ui->check();
};

MasterRenderer::~MasterRenderer()
{
	delete fb_scene;
	delete fb_ui;
};

void MasterRenderer::WindowSizeChangeCallback()
{
	glm::vec2 vid_mode = b_Engine::getVidMode();
	fb_scene->changeSize(vid_mode.x, vid_mode.y);
	fb_ui->changeSize(vid_mode.x, vid_mode.y);
	glViewport(0, 0, vid_mode.x, vid_mode.y);
};

Framebuffer* MasterRenderer::getSceneFramebuffer() const
{
	return fb_scene;
};

Framebuffer* MasterRenderer::getUIFramebuffer() const
{
	return fb_ui;
};


void MasterRenderer::Clear()
{
	b_draw_calls = 0;
	b_total_vertices = 0;

	// Render lines if render mode is RENDER_WIRE, else render solid
	if (b_render_mode == RENDER_WIRE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		b_Context::DisableFaceCulling();
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	};
	
	b_Context::Clear();
	fb_scene->clear(glm::vec4(0.f));
	fb_ui->clear(glm::vec4(0.f));
};

void MasterRenderer::Flush()
{
	b_Context::DisableDepthTest();
	b_Context::DisableFaceCulling();
	
	p_main.use();

	p_main.set1i(0, "_scene");
	p_main.set1i(1, "_ui");
	
	b_Texture::bindToSlot(0u, this->fb_scene->getColorAttachment());
	b_Texture::bindToSlot(1u, this->fb_ui->getColorAttachment());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	b_AssetManager::getMeshBasicQuad()->Draw();
};

void MasterRenderer::switchRenderMode()
{
	int current_mode = (int)b_render_mode;
	current_mode = (current_mode + 1) % 4;
	b_render_mode = (RenderMode)current_mode;
	LOG_MSG("Render mode switched to %u", b_render_mode);
};