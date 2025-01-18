#include "entity_renderer.h"

using namespace b_GameObject;

EntityRenderer::EntityRenderer(Framebuffer* v)
{
	fb_scene = v;
	p_main.InitVertexAndFragment("b_standart");
};

void EntityRenderer::Render(
	const std::vector<Entity *>& list,
	Camera* cam
)
{
	// Bind 3D framebuffer
	fb_scene->bind();
	p_main.use();
	// Set sampler2D slot to index 0
	p_main.set1i(0, "u_texture");

	b_Context::EnableDepthTest();

	for (Entity* e : list)
	{
		/* =============== Position ============ */
		// Model
		e->transform.UpdateModel();
		p_main.setmat4(e->transform.getModel(), "u_model");

		// Camera
		p_main.setmat4(
			cam->getProjectionMatrix(), "u_projection"
		);
		p_main.setmat4(
			cam->getViewMatrix(), "u_view"
		);
		p_main.set3f(
			cam->getPosition(), "u_camera_position"
		);

		/* =============== Color =============== */
		p_main.set1i(0, "u_depth");
		p_main.set1i(0, "u_rnormal");
		p_main.set3f(COLOR_NULL, "u_transperent_pixel");

		const EntityAppearance& a = e->getAppearance();
		const glm::vec4& c = a.getColor();

		switch (b_render_mode)
		{
			// Textured rendering
			case RENDER_TEXTURED:
				
				if (a.hasTexture())
				{
					b_Texture::Texture* t = a.getTexture();
					p_main.set1i(1, "u_use_texturing");
					p_main.set1f(a.getUVScaling(), "u_uv_scaling");
					p_main.set1i(
						(int)t->use_transperency_mask,
						"u_use_transperency_mask"
					);
					// Bind texture to slot 0
					b_Texture::bindToSlot(0u, t);
				}
				else
				{
					p_main.set1i(0, "u_use_texturing");
				}
				p_main.set4f(c, "u_color");
			break;

			// Wireframe rendering
			case RENDER_WIRE:
				p_main.set1i(0, "u_use_texturing");
				p_main.set4f(c, "u_color");
			break;

			// Depth rendering
			case RENDER_DEPTH:
				p_main.set1i(1, "u_depth");	
			break;

			// Normals rendering
			case RENDER_NORMAL:
				p_main.set1i(1, "u_rnormal");
			break;
		};

		if (a.getFaceCullingUsing() && b_render_mode != RENDER_WIRE)
			b_Context::EnableFaceCulling();

		e->getMesh()->Draw();
	};
};