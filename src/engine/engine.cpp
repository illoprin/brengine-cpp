#include "engine.h"

static void errorCallback(int error, const char* desc)
{
	fprintf(stderr, "Error: %s\n", desc);
};

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Engine* e_ptr = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
	if (e_ptr != NULL)
	{
		if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		{
			printf("Closing...\n");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		b_UserKeyCallback user_func = e_ptr->getIO()->getKeyCallback();
		if (user_func != nullptr) user_func(key, action, mods);
	}
};

static void mouseMotionCallback(GLFWwindow* window, double x, double y)
{
	Engine* e_ptr = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
	if (e_ptr != NULL)
	{
    	e_ptr->getIO()->updateMouse(x, y);
		b_UserMouseMotionCallback user_func = e_ptr->getIO()->getMouseMotionCallback();
		if (user_func != nullptr) user_func(x, y);
	}
};

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Engine* e_ptr = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
	if (e_ptr != NULL)
	{
		b_UserMouseButtonCallback user_func = e_ptr->getIO()->getMouseButtonCallback();
		if (user_func != nullptr) user_func(button, action);
	}
};

static void resizeCallback(GLFWwindow* window, int width, int height)
{
	Engine* e_ptr = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
	if (e_ptr != NULL)
	{

	}
	e_ptr->setVidMode((unsigned)width, (unsigned)height);

	printf("Window resized: %d %d\n", e_ptr->getVidMode().x, e_ptr->getVidMode().y);
	glViewport(0, 0, width, height);
};

Engine::Engine()
{
	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Set MSAA parametres
	glfwWindowHint(GLFW_SAMPLES, 4);

	this->initWindow();
	// Create pointer to Logger instance 
	this->log = new Log("tmp");
	// Create pointer to Renderer instance
	this->renderer = new Renderer(this->log, window);
	// Create poiner to Clock instance
	this->clock = new Clock();
	this->initIO();
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
	this->vid_mode = glm::ivec2(WIN_WIDTH, WIN_HEIGHT);

	if (!this->window)
	{
		printf("Could not create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	};
	glfwFocusWindow(this->window);

	glfwSetWindowUserPointer(this->window, this);
};

void Engine::initIO()
{
	this->input = new Input(this->log);
	glfwSetErrorCallback(errorCallback);

	glfwSetKeyCallback(this->window, keyCallback);
	glfwSetMouseButtonCallback(this->window, mouseButtonCallback);
	glfwSetCursorPosCallback(this->window, mouseMotionCallback);
	glfwSetWindowSizeCallback(this->window, resizeCallback);
};

void Engine::setVidMode(unsigned w, unsigned h)
{
	this->vid_mode.x = w;
	this->vid_mode.y = h;
};

glm::ivec2 Engine::getVidMode()
{
	return this->vid_mode;
};
GLFWwindow* Engine::getWindow() const
{
	return this->window;
};
Log* Engine::getLogger() const
{
	return this->log;
};

Clock* Engine::getClock() const
{
	return this->clock;
};
Renderer* Engine::getRenderer() const
{
	return this->renderer;
};
Input* Engine::getIO() const
{
	return this->input;
};

/* ================== Update Methods ================== */
void Engine::prepare()
{
	this->clock->start();
};
void Engine::update(Scene& scene)
{
	scene.update();
};
void Engine::render(Scene& scene)
{
	renderer->render(scene);
};
void Engine::close()
{
	glfwSwapBuffers(this->window);
	this->clock->end();
};
/* ==================================================== */

Engine::~Engine()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	delete this->renderer;
	delete this->clock;
	delete this->log;
	delete this->input;
};