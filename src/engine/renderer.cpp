#include "renderer.h"

Renderer::Renderer(Log* log, GLFWwindow* win)
{
	this->window = win;
	this->log = log;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwMakeContextCurrent(win);

	// glfwSwapInterval(1); /* Creates window motion slowdown on X11 */

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
};

void Renderer::ctxPrepare()
{
	glEnable(GL_DEPTH_TEST); // Enable depth detection
	glEnable(GL_CULL_FACE); // Enable backface culling
	glEnable(GL_BLEND); // Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set alpha blending model
	glCullFace(GL_CCW); // Set backface culling model - vertices order is counter clockwise
	printf("OpenGL parametres inited\n");
};

Renderer::~Renderer()
{
	// ... 
};

void Renderer::start()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.11f, 0.11f, 0.11f, 1.f);
};

void Renderer::render(Scene& scene)
{
	for (Entity* entity : scene.getEntities())
	{
		entity->getProgram()->use();
		entity->getMesh()->draw();
	}
};

void Renderer::end()
{
	glfwSwapBuffers(window);
};

