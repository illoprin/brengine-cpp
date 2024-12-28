#include "engine.h"

static void errorCallback(int error, const char* desc)
{
	fprintf(stderr, "Error: %s\n", desc);
};

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
	{
		printf("Closing...\n");
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
};

static void resizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

Engine::Engine()
{
	initWindow();
	// Create pointer to Logger instance 
	this->log = new Log("tmp");
	// Create pointer to Renderer instance
	this->renderer = new Renderer(this->log, window);
	// Create poiner to Clock instance
	this->clock = new Clock();
	// Create default shader program
	this->p_default = new Program(this->log, "b_default");
};

void Engine::initWindow()
{
	if (!glfwInit())
	{
		printf("Could not init GLFW\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	this->window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);

	if (!this->window)
	{
		printf("Could not create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	};

	glfwFocusWindow(this->window);

	glfwSetKeyCallback(this->window, keyCallback);
	glfwSetErrorCallback(errorCallback);
	glfwSetWindowSizeCallback(this->window, resizeCallback);
};

GLFWwindow* Engine::getWindow() const
{
	return this->window;
};

Log* Engine::getLogger() const
{
	return this->log;
}

Program* Engine::getDefaultProgram() const
{
	return this->p_default;
};

/* ================== Update Methods ================== */
void Engine::prepare()
{
	this->clock->start();
};
void Engine::update(Scene& scene)
{
	glfwPollEvents();
	scene.update();
};
void Engine::render(Scene& scene)
{
	renderer->start();
	renderer->render(scene);
	renderer->end();
};
void Engine::close()
{
	this->clock->end();
};
/* ==================================================== */

Engine::~Engine()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	delete this->p_default;
	delete this->renderer;
	delete this->clock;

	delete this->log;
};