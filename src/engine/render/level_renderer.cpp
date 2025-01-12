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
	// Textures
	b_Texture::TextureStorage3D* ts = level->getTextureStorage();
	b_Texture::bindToSlot(0, ts);
	// Color palette
	b_Texture::TextureStorage3D* ps = level->getPalette();
	b_Texture::bindToSlot(0, ps);

	/* ========= Position ========= */
	p_main.setmat4(
		c->getViewMatrix(), "u_view"
	);
	p_main.setmat4(
		c->getProjectionMatrix(), "u_view"
	);
	p_main.set3f(
		c->getPosition(), "u_camera_position"
	);

	level->getMesh()->Draw();
	
	p_main.set1i(0, "u_texture");
	p_main.set1i(1, "u_palette");

	fb_scene->unbind();
};
