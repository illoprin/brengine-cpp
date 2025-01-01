#include "renderer.h"

Renderer::Renderer(Log* log, GLFWwindow* win)
{
	this->window = win;
	this->log = log;
	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Set MSAA parametres
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwMakeContextCurrent(win); // Now OpenGL draws stuff in this window

	glfwSwapInterval(1); /* Creates window motion slowdown on X11 */

	if (glewInit() != GLEW_OK)
	{
		printf("Could not init GLEW\n");
		exit(EXIT_FAILURE);
	}

	glewExperimental = true;

	int width, height;
	glfwGetWindowSize(win, &width, &height);
	glViewport(0, 0, width, height);

	this->log->logf(
		"[INFO] Renderer - OpenGL initialized, render mode is %d %d\n",
		width, height);

	this->ctxPrepare();
	this->initPrograms();
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

void Renderer::initPrograms()
{
	// Use this shader_program to render 2D textured stuff
	this->p_flat = new Program{this->log};
	this->p_flat->InitVertexAndFragment("b_flat");
	
	// Use this to shader_program 3D textured stuff
	this->p_standart = new Program{this->log};
	this->p_standart->InitVertexAndFragment("b_standart");
};

void Renderer::start()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.11f, 0.11f, 0.11f, 1.f);
};

void Renderer::render(Scene& scene)
{
	switch(scene.getType())
	{
		case SCENE_STANDART:
			this->render_3d(scene);
		break;
		case SCENE_FLAT:
			this->render_flat(scene);
		break;
	}
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
		|| this->r_mode == RENDER_DEPTH)
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
	this->ctxDisableFaceCulling();
	this->ctxDisableDepthTest();

	// Render lines if mode is RENDER_WIRE, else render solid triangles
	e->getMesh()->draw(
		r_mode != RENDER_WIRE ? GL_TRIANGLES : GL_LINE_STRIP
	);
};
void Renderer::render_3d_entity(Entity* e, Program* p, Camera* c)
{
	p->use();
	
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
	switch (this->r_mode)
	{
		// Textured rendering
		case RENDER_TEXTURED:
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
	};

	this->ctxEnableFaceCulling();
	this->ctxEnableDepthTest();

	// Render lines if render mode is RENDER_WIRE, else render solid triangles
	e->getMesh()->draw(
		r_mode != RENDER_WIRE ? GL_TRIANGLES : GL_LINE_STRIP
	);
};

void Renderer::setRenderMode(RenderMode mode)
{
	this->r_mode = mode;
};

void Renderer::switchRenderMode()
{
	int current_mode = (int)this->r_mode;
	current_mode = (current_mode + 1) % 3;
	this->r_mode = (RenderMode)current_mode;
	log->logf(
		"[INFO] Renderer - render mode switched to %d\n", 
		(int)this->r_mode);
};

Program* Renderer::getProgramFlat() const
{
	return this->p_flat;
}

Program* Renderer::getProgramStandart() const
{
	return this->p_standart;
}

Renderer::~Renderer()
{
	delete this->p_flat;
	delete this->p_standart;
};