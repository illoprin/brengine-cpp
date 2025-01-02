#include "renderer.h"

static float quad_data[] = {
	1.f, -1.f, 1.f, 0.f,
	-1.f, -1.f, 0.f, 0.f,
	1.f, 1.f, 1.f, 1.f,
	-1.f, -1.f, 0.f, 0.f,
	-1.f, 1.f, 0.f, 1.f,
	1.f, 1.f, 1.f, 1.f,
};

Renderer::Renderer(Log* log, GLFWwindow* win)
{
	this->window = win;
	this->log	 = log;
	glfwMakeContextCurrent(win); // Now OpenGL draws stuff in this window

	glfwSwapInterval(1); /* Creates window motion slowdown on X11 */

	if (glewInit() != GLEW_OK)
	{
		printf("Could not init GLEW\n");
		exit(EXIT_FAILURE);
	};

	glewExperimental = true;

	int width, height;
	glfwGetWindowSize(win, &width, &height);
	glViewport(0, 0, width, height);

	this->log->logf(
		"[INFO] Renderer - OpenGL initialized, render mode is %d %d\n",
		width, height);

	this->ctxPrepare();

	this->init_programs();
	this->init_framebuffers();
	this->init_help_meshes();
	
	this->setRenderMode(RENDER_TEXTURED);
};

void Renderer::ctxPrepare()
{
	glEnable(GL_MULTISAMPLE); // Enable MSAA
	glEnable(GL_BLEND); // Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set alpha blending model
};

void Renderer::ctxEnableFaceCulling()
{
	glEnable(GL_CULL_FACE); // Enable backface culling
	glCullFace(GL_BACK); // Set backface culling model - vertices order is counter clockwise
	glFrontFace(GL_CCW); // Face culling - front face is in clockwise
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
	this->p_flat = new Program{this->log};
	this->p_flat->InitVertexAndFragment("b_flat");
	
	// Use this to render 3D textured/colored stuff
	this->p_standart = new Program{this->log};
	this->p_standart->InitVertexAndFragment("b_standart");

	// Use this to render screenquad
	this->p_mixer = new Program{this->log};
	this->p_mixer->InitVertexAndFragment("b_godmixer");
};
void Renderer::init_framebuffers()
{
	this->fb_scene = new Framebuffer(this->log, "fb_scene");
	this->fb_scene->initColorAttachment(WIN_WIDTH, WIN_HEIGHT);
	this->fb_scene->initDepthAttachment(WIN_WIDTH, WIN_HEIGHT);
	this->fb_scene->check();

	this->fb_ui = new Framebuffer(this->log, "fb_ui");
	this->fb_ui->initColorAttachment(WIN_WIDTH, WIN_HEIGHT);
	this->fb_ui->check();
};
void Renderer::init_help_meshes()
{
	this->m_squad = new BaseMesh{this->log, "screen_quad"};

	this->m_squad->addb(quad_data, 24 * sizeof(float));
	GLuint lb = this->m_squad->getLastBuffer();
	this->m_squad->addattr(
		lb, 2, 4 * sizeof(float), 0);
	this->m_squad->addattr(
		lb, 2, 4 * sizeof(float), 2 * sizeof(float));
	this->m_squad->setTotal(6); // 6 vertices
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

void Renderer::RenderScene(Scene& scene)
{
	// Render lines if render mode is RENDER_WIRE, else render solid
	if (this->r_mode == RENDER_WIRE)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	switch(scene.getType())
	{
		case SCENE_STANDART:
			this->ctxEnableFaceCulling();
			this->ctxEnableDepthTest();

			this->fb_scene->bind();
			this->render_3d(scene);
		break;
		case SCENE_FLAT:
			this->ctxDisableFaceCulling();
			this->ctxDisableDepthTest();

			this->fb_ui->bind();
			this->render_flat(scene);
		break;
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
	this->m_squad->draw();
	glActiveTexture(GL_TEXTURE0);
	
	glfwSwapBuffers(this->window);
};

void Renderer::render_flat(Scene& scene)
{
	int width, height;
	glfwGetWindowSize(this->window, &width, &height);
	for (Entity* entity : scene.getEntities())
	{
		if (entity->hasView())
		{
			Program* prog = entity->getProgram();
			this->render_flat_entity(entity, prog, width, height);
		}
	};
};

void Renderer::render_3d(Scene& scene)
{
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

void Renderer::render_flat_entity(Entity* e, Program* p, int w, int h)
{
	p->use();
	p->set1i(0, "u_texture");


	// In position

	/* 2D - Model view */
	e->transform.UpdateModel();
	p->setmat4(e->transform.getModel(), "u_model");

	/* 2D - Ortho Projection */
	float aspect = (float)w / (float)h;
	glm::mat4 ortho_projection = glm::ortho(
		-aspect, aspect, // X
		-1.f, 1.f, // Y
		0.f, 1.f // Near Far
	);
	p->setmat4(
		ortho_projection, "u_projection"
	);

	/* =============== Color =============== */
	// 2D Renderer - textured rendering
	if (this->r_mode == RENDER_TEXTURED
		|| this->r_mode == RENDER_DEPTH
		|| this->r_mode == RENDER_NORMAL)
	{
		if (e->getTexture() != nullptr)
		{
			p->set1i(1, "u_use_texturing");
			e->getTexture()->bind();
		}
		else
		{
			p->set1i(0, "u_use_texturing");
		}
		p->set4f(e->getColor(), "u_color");
	}
	// 2D Renderer - Wireframe rendering
	else if (this->r_mode == RENDER_WIRE)
	{
		p->set1i(0, "u_use_texturing");
		p->set4f(glm::vec4(1.f, 0.09f, 0.79f, 1.f), "u_color");
		p->set1f(1.f, "u_alpha");
	}

	// Render lines if mode is RENDER_WIRE, else render solid triangles
	e->getMesh()->draw(
		r_mode != RENDER_WIRE ? GL_TRIANGLES : GL_LINE_STRIP
	);
};
void Renderer::render_3d_entity(Entity* e, Program* p, Camera* c)
{
	p->use();
	p->set1i(0, "u_texture");
	
	// In position
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
	switch (this->r_mode)
	{
		// Textured rendering
		case RENDER_TEXTURED:
			if (e->getTexture() != nullptr)
			{
				p->set1i(1, "u_use_texturing");
				p->set1f(e->getUVScaling(), "u_uv_scaling");
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

	e->getMesh()->draw();
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
	log->logf(
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
	delete this->m_squad;
	delete this->fb_scene;
	delete this->fb_ui;
	delete this->p_flat;
	delete this->p_standart;
	delete this->p_mixer;
};