#include "renderer.h"

#include "../engine.h"

using namespace b_GUI;
using namespace b_GameObject;

Renderer::Renderer()
{
	glfwMakeContextCurrent(b_Engine::getWindow()); // Now OpenGL draws stuff in this window

	// Enable VSync
	glfwSwapInterval(1); /* Creates window motion slowdown on X11 */

	if (glewInit() != GLEW_OK)
	{
		printf("Could not init GLEW\n");
		exit(EXIT_FAILURE);
	};

	glewExperimental = true;

	b_log->logf(
		"[INFO] Renderer - OpenGL initialized, render mode is %d %d\n",
		b_Engine::getVidMode().x, b_Engine::getVidMode().y);

	this->ctxPrepare();

	this->init_programs();
	this->init_framebuffers();

	this->setRenderMode(RENDER_TEXTURED);
};

void Renderer::FramebufferSizeChange()
{
	glm::ivec2 vid_mode = b_Engine::getVidMode();
	this->fb_scene->changeSize(vid_mode.x, vid_mode.y);
	this->fb_ui->changeSize(vid_mode.x, vid_mode.y);
	glViewport(0, 0, vid_mode.x, vid_mode.y);
};

void Renderer::ctxPrepare()
{
	// glEnable(GL_MULTISAMPLE); // MSAA is not working, because app use multiple framebuffers
	glEnable(GL_BLEND); // Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set alpha blending model
};

void Renderer::ctxEnableFaceCulling()
{
	glEnable(GL_CULL_FACE); // Enable backface culling
	glCullFace(GL_BACK); // Set backface culling model - vertices order is counter clockwise
	glFrontFace(GL_CCW); // Face culling - front face is in clockwise order
};

void Renderer::ctxDisableFaceCulling()
{
	glDisable(GL_CULL_FACE);
};

void Renderer::ctxEnableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
};
void Renderer::ctxDisableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
};

void Renderer::init_programs()
{
	// Use this to render 2D textured/colored stuff
	this->p_flat = new Program{};
	this->p_flat->InitVertexAndFragment("b_flat");
	
	// Use this to render 3D textured/colored stuff
	this->p_standart = new Program{};
	this->p_standart->InitVertexAndFragment("b_standart");

	// Use this to render screenquad
	this->p_mixer = new Program{};
	this->p_mixer->InitVertexAndFragment("b_godmixer");
};
void Renderer::init_framebuffers()
{
	this->fb_scene = new Framebuffer("fb_scene");
	this->fb_scene->initColorAttachment(WIN_WIDTH, WIN_HEIGHT);
	this->fb_scene->initDepthAttachment(WIN_WIDTH, WIN_HEIGHT);
	this->fb_scene->check();

	this->fb_ui = new Framebuffer("fb_ui");
	this->fb_ui->initColorAttachment(WIN_WIDTH, WIN_HEIGHT);
	this->fb_ui->check();
};

void Renderer::ctxBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void Renderer::ctxClear()
{
	this->ctxBind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.11f, 0.11f, 0.11f, 1.f);
};

void Renderer::ClearCanvas()
{
	this->ctxClear();
	this->fb_scene->clear(glm::vec4(0.f));
	this->fb_ui->clear(glm::vec4(0.f));
};

void Renderer::Update()
{
	float aspect = b_Engine::getAspect();
	this->m_ortho = glm::ortho(
		-aspect, aspect, // X
		-1.f, 1.f,       // Y
		0.f, 1.f         // Near Far
	);
};

void Renderer::RenderScene(Scene3D& scene)
{
	// Render lines if render mode is RENDER_WIRE, else render solid
	if (this->r_mode == RENDER_WIRE)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		this->ctxDisableFaceCulling();
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
	this->ctxEnableDepthTest();

	this->fb_scene->bind();
	for (Entity* entity : scene.getEntities())
	{
		if (entity->hasView())
		{
			Program* prog = entity->getProgram();
			Camera* cam = scene.getCameraMain();
			this->render_3d_entity(entity, prog, cam);
		};
	};
};

void Renderer::RenderUI(GUIScene& s)
{
	this->ctxDisableDepthTest();
	this->ctxDisableFaceCulling();
	float aspect = b_Engine::getAspect();
	this->fb_ui->bind();

	this->p_flat->use();
	// Send projection matrix to flat ui shader
	this->p_flat->setmat4(
		this->m_ortho, "u_projection"
	);
	for (GUIItem* i : s.getItems())
	{
		i->update(); // Update item before drawing

		this->p_flat->set1i(0, "u_texture"); // Texture index

		/* =============== Position ============ */
		// Form model matrix based on item info
		// TODO: Compute item position based on window size
		glm::mat4 model{1};
		glm::vec3 pos {i->getPosition().x * aspect, i->getPosition().y, 0};
		glm::vec3 scl {i->getScaling().x, i->getScaling().y, 0};
		model = glm::translate(model, pos);	
		model = glm::rotate(model, i->getRotation(), {0.f, 0.f, 1.f});
		model = glm::scale(model, (scl * 0.25f)); // 0.25 is unit scale - 1/4 of screen
		// Send model matrix to shader
		this->p_flat->setmat4(model, "u_model");

		/* =============== Color =============== */
		this->p_flat->set4f(i->getColor(), "u_color");
		this->p_flat->set1i(0, "u_is_text");
		this->p_flat->set1i(0, "u_use_texturing");
		this->p_flat->set3f(COLOR_NULL, "u_transperent_pixel");
		switch (i->getType())
		{
			case GUI_SHAPE:
				b_AssetManager::getMeshBasicQuad()->Draw();
			break;

			case GUI_IMAGE:
				if (this->r_mode != RENDER_WIRE)
				{
					i->getTexture()->bind();
					this->p_flat->set1i(
						(int)i->getTexture()->use_transperency_mask,
						"u_use_transperency_mask"
					);
					this->p_flat->set1i(1, "u_use_texturing");
				}
				b_AssetManager::getMeshBasicQuad()->Draw();
			break;
			
			case GUI_TEXT:
				if (i->need_redraw)
				{
					b_GUI::StringToMesh((GUIText *)i);
					i->need_redraw = false;
				}
				if (this->r_mode != RENDER_WIRE)
				{
					this->p_flat->set1i(1, "u_is_text");
					this->p_flat->set1i(1, "u_use_texturing");
					i->getFont()->t_atlas->bind();
				}
				i->getMesh()->Draw();
			break;
		};
	}

};

void Renderer::Flush()
{
	this->ctxClear();

	this->ctxDisableDepthTest();
	
	this->p_mixer->use();

	this->p_mixer->set1i(0, "_scene");
	this->p_mixer->set1i(1, "_ui");
	
	b_Texture::bindToSlot(0, this->fb_scene->getColorAttachment());
	b_Texture::bindToSlot(1, this->fb_ui->getColorAttachment());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	b_AssetManager::getMeshBasicQuad()->Draw();
	glActiveTexture(GL_TEXTURE0);
	
	glfwSwapBuffers(b_Engine::getWindow());
};

void Renderer::render_3d_entity(Entity* e, Program* p, Camera* c)
{
	p->use();
	p->set1i(0, "u_texture");
	
	/* =============== Position ============ */
	e->transform.UpdateModel();
	p->setmat4(e->transform.getModel(), "u_model");
	
	p->setmat4(
		c->getProjectionMatrix(), "u_projection"
	);
	p->setmat4(
		c->getViewMatrix(), "u_view"
	);
	p->set3f(
		c->getPosition(), "u_camera_position"
	);

	/* =============== Color =============== */
	p->set1i(0, "u_depth");
	p->set1i(0, "u_rnormal");
	p->set3f(COLOR_NULL, "u_transperent_pixel");
	switch (this->r_mode)
	{
		// Textured rendering
		case RENDER_TEXTURED:
			if (e->getTexture() != nullptr)
			{
				p->set1i(1, "u_use_texturing");
				p->set1f(e->getUVScaling(), "u_uv_scaling");
				p->set1i(
					(int)e->getTexture()->use_transperency_mask,
					"u_use_transperency_mask"
				);
				e->getTexture()->bind();
			}
			else
			{
				p->set1i(0, "u_use_texturing");
			}
			p->set4f(e->getColor(), "u_color");
		break;

		// Wireframe rendering
		case RENDER_WIRE:
			p->set1i(0, "u_use_texturing");
			p->set4f(glm::vec4(1.f, 0.09f, 0.79f, 1.f), "u_color");
		break;

		// Depth rendering
		case RENDER_DEPTH:
			p->set1i(1, "u_depth");	
		break;

		// Normals rendering
		case RENDER_NORMAL:
			p->set1i(1, "u_rnormal");
		break;
	};
	
	// OpenGL statements preparations
	if (e->getFaceCulling() && this->r_mode != RENDER_WIRE)
		this->ctxEnableFaceCulling();
	else
		this->ctxDisableFaceCulling();
	

	e->getMesh()->Draw();
};

void Renderer::setRenderMode(RenderMode mode)
{
	this->r_mode = mode;
};

void Renderer::switchRenderMode()
{
	int current_mode = (int)this->r_mode;
	current_mode = (current_mode + 1) % 4;
	this->r_mode = (RenderMode)current_mode;
	b_log->logf(
		"[INFO] Renderer - render mode switched to %d\n", 
		(int)this->r_mode);
};

Program* Renderer::getProgramFlat() const
{
	return this->p_flat;
};

Program* Renderer::getProgramStandart() const
{
	return this->p_standart;
};
Program* Renderer::getProgramCanvas() const
{
	return this->p_mixer;
};

Renderer::~Renderer()
{
	delete this->fb_scene;
	delete this->fb_ui;
	delete this->p_flat;
	delete this->p_standart;
	delete this->p_mixer;
};