#include "level_renderer.h"

using namespace b_Level;

LevelRenderer::LevelRenderer(Framebuffer* v)
{
	fb_scene = v;
	p_main.InitVertexAndFragment("b_level");
};

void LevelRenderer::Render(b_Level::Level* level, Camera* c)
{
	fb_scene->bind();
	p_main.use();

	/* ========= Position ========= */
	p_main.setmat4(
		c->getViewMatrix(), "u_view"
	);
	p_main.setmat4(
		c->getProjectionMatrix(), "u_projection"
	);
	p_main.set3f(
		c->getPosition(), "u_camera_position"
	);

	/* ==========  Color  ========= */
	p_main.set1i(0, "u_color_only");
	p_main.set1i(0, "u_depth");
	p_main.set1i(0, "u_normals");

	p_main.set3f(COLOR_NULL, "u_transperent_pixel");

	// Textures
	b_Texture::TextureStorage3D* ts = level->getTextureStorage();
	b_Texture::bindToSlot(0, ts);

	// Color palette
	b_Texture::TextureStorage3D* ps = level->getPalette();
	b_Texture::bindToSlot(1, ps);

	b_Texture::bindToSlot(2, b_AssetManager::getTextureNull());
	
	// Send layers count (needs for UV calculating)
	p_main.set1i(ts->layers, "u_texture_layers");
	p_main.set1i(ps->layers, "u_palette_layers");

	// Switch by render modes
	switch (b_render_mode)
	{
		case RENDER_NORMAL:
			p_main.set1i(1, "u_normals");
		break;
		case RENDER_DEPTH:
			p_main.set1i(1, "u_depth");
		break;
		case RENDER_WIRE:
			p_main.set1i(1, "u_color_only");
			b_Context::DisableFaceCulling();
		break;
		case RENDER_TEXTURED:
			// Set sampler2D slots
			p_main.set1i(0, "u_texture");
			p_main.set1i(1, "u_palette");
			p_main.set1i(2, "u_texture_null");

			b_Context::EnableFaceCulling();
		break;
	}
	b_Context::EnableDepthTest();
	

	level->getMesh()->Draw();
	glActiveTexture(GL_TEXTURE0);
	fb_scene->unbind();
};
