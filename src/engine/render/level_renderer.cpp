#include "level_renderer.h"

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

	p_main.set3f(COLOR_NULL, "u_transperent_pixel");

	// Textures
	b_Texture::TextureStorage3D* ts = level->getTextureStorage();
	b_Texture::bindToSlot(0, ts);
	p_main.set1i(0, "u_texture");

	// Color palette
	b_Texture::TextureStorage3D* ps = level->getPalette();
	b_Texture::bindToSlot(1, ps);
	p_main.set1i(1, "u_palette");

	b_Texture::bindToSlot(2, b_AssetManager::getTextureNull());
	p_main.set1i(2, "u_texture_null");

	b_Context::EnableDepthTest();
	b_Context::DisableFaceCulling();

	level->getMesh()->Draw();
	fb_scene->unbind();
};
