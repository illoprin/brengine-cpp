#include "renderer.h"

Renderer::Renderer(Log* log, GLFWwindow* win)
{
	
	this->window = win;
	this->log = log;
	glfwMakeContextCurrent(win);

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

	this->ctxPrepare();
	this->initPrograms();
};

void Renderer::ctxPrepare()
{
	glEnable(GL_DEPTH_TEST); // Enable depth detection
	glEnable(GL_MULTISAMPLE); // Enable MSAA

	glEnable(GL_BLEND); // Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set alpha blending model

	// glEnable(GL_CULL_FACE); // Enable backface culling
	glCullFace(GL_BACK); // Set backface culling model - vertices order is counter clockwise
	glFrontFace(GL_CCW); // Face culling - front face is in Conter clockwise

	printf("OpenGL parametres inited\n");
};

void Renderer::initPrograms()
{
	// Use this shader_program to render 2D textured stuff
	this->p_flat = new Program{this->log, "b_flat"};
	// Use this to shader_program 3D textured stuff
	this->p_standart = new Program{this->log, "b_standart"};
};

void Renderer::start()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.11f, 0.11f, 0.11f, 1.f);
};

void Renderer::render(Scene& scene)
{
	// Rendering entities
	for (Entity* entity : scene.getEntities())
	{
		Program* p_current = entity->getProgram();
		p_current->use();

		/* =========== Positions Uniforms ========= */
		p_current->setmat4(entity->getModelMatrix(), "u_model");
		// If scene is 3D -> send camara info
		if (scene.getType() == SCENE_STANDART)
		{
			Camera* s_cam = scene.getCameraMain();
			
			p_current->setmat4(
				s_cam->getProjectionMatrix(), "u_projection"
			);
			p_current->setmat4(
				s_cam->getViewMatrix(), "u_view"
			);
			
			p_current->set3f(
				s_cam->getPosition(), "u_camera_position"
			);
		};

		/* ============ Color Uniforms ============ */
		if (entity->getTexture() != nullptr)
		{
			p_current->set1i(1, "u_use_texturing");
			entity->getTexture()->bind();
		}
		else
		{
			p_current->set1i(0, "u_use_texturing");
			p_current->set3f(entity->getColor(), "u_color");
		}
		// Send alpha value
		p_current->set1f(entity->getAlpha(), "u_alpha");

		entity->getMesh()->draw();
	};
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