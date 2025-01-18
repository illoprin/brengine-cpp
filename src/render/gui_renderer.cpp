#include "gui_renderer.h"

#include <core/engine.h>

GUIRenderer::GUIRenderer(Framebuffer* fb)
{
	fb_ui = fb;
	// Init shader program
	p_main.InitVertexAndFragment("b_flat");
};


void GUIRenderer::Render(
	const std::vector<b_GUI::GUIItem *>& list
)
{
	b_Context::DisableDepthTest();
	b_Context::DisableFaceCulling();
	float aspect = b_Engine::getAspect();
	// Bind UI framebuffer
	fb_ui->bind();
	// Use flat program
	p_main.use();

	// Update ortho projection matrix
	this->m_ortho = glm::ortho(
		-aspect, aspect, // X
		-1.f, 1.f,       // Y
		0.f, 1.f         // Near Far
	);

	p_main.setmat4(this->m_ortho, "u_projection");

	// Iterate over the list
	for (b_GUI::GUIItem* item : list)
	{
		// Update item before drawing
		item->update();

		/* =============== Position ============ */
		// Form model matrix based on item info
		// TODO: Compute item position based on window size
		glm::mat4 model{1};
		glm::vec3 pos {item->getPosition().x * aspect, item->getPosition().y, 0};
		glm::vec3 scl {item->getScaling().x, item->getScaling().y, 0};
		model = glm::translate(model, pos);	
		model = glm::rotate(model, item->getRotation(), {0.f, 0.f, 1.f});
		model = glm::scale(model, (scl * 0.25f)); // 0.25 is unit scale - 1/4 of screen
		// Send model matrix to shader
		p_main.setmat4(model, "u_model");

		/* =============== Color =============== */
		p_main.set4f(item->getColor(), "u_color");
		p_main.set1i(0, "u_is_text");
		p_main.set1i(0, "u_use_texturing");
		p_main.set3f(COLOR_NULL, "u_transperent_pixel");
		p_main.set1i(0, "u_texture"); // Set sampler2D bind index

		switch (item->getType())
		{
			case GUI_SHAPE:
				b_AssetManager::getMeshBasicQuad()->Draw();
			break;

			case GUI_IMAGE:
				if (b_render_mode != RENDER_WIRE)
				{
					p_main.set1i(
						(int)item->getTexture()->use_transperency_mask,
						"u_use_transperency_mask"
					);
					p_main.set1i(1, "u_use_texturing");
					b_Texture::bindToSlot(0u, item->getTexture());
				}
				b_AssetManager::getMeshBasicQuad()->Draw();
			break;

			case GUI_TEXT:
				// Update text mesh if it needs to redraw
				if (item->need_redraw)
				{
					b_GUI::StringToMesh((b_GUI::GUIText *)item);
					item->need_redraw = false;
				}
				if (b_render_mode != RENDER_WIRE)
				{
					p_main.set1i(1, "u_is_text");
					p_main.set1i(1, "u_use_texturing");
					
					b_Texture::bindToSlot(0u, &item->getFont()->t_atlas);
				}
				item->getMesh()->Draw();
			break;
		};
	};

	// Unbind UI framebuffer
	fb_ui->unbind();
}