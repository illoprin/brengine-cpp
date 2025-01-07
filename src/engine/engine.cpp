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
		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_ESCAPE)
			{
				printf("Closing...\n");
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			
			if (key == GLFW_KEY_TAB) e_ptr->toggleGameMode();

			if (key == GLFW_KEY_F5) e_ptr->getRenderer()->switchRenderMode();
			if (key == GLFW_KEY_F2) e_ptr->takeScreenshot();
		}
		// Execute user function
		b_UserKeyCallback user_func = e_ptr->getIO()->getKeyCallback();
		if (user_func != nullptr) user_func(key, action, mods);
	}
};

static void mouseMotionCallback(GLFWwindow* window, double x, double y)
{
	Engine* e_ptr = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
	if (e_ptr != NULL)
	{
		// Execute user function
		b_UserMouseMotionCallback user_func = e_ptr->getIO()->getMouseMotionCallback();
		if (user_func != nullptr) user_func(x, y);
	}
};

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	Engine* e_ptr = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
	if (e_ptr != NULL)
	{
		// Execute user function
		b_UserMouseButtonCallback user_func = e_ptr->getIO()->getMouseButtonCallback();
		if (user_func != nullptr) user_func(button, action);
	}
};

static void resizeCallback(GLFWwindow* window, int width, int height)
{
	Engine* e_ptr = reinterpret_cast<Engine *>(glfwGetWindowUserPointer(window));
	if (e_ptr != NULL)
	{
		e_ptr->setVidMode((unsigned)width, (unsigned)height);
		printf("Window resized: %d %d\n", e_ptr->getVidMode().x, e_ptr->getVidMode().y);
		glViewport(0, 0, width, height);

	}
};

Engine::Engine()
{
	b_Files::InitFilesystem();

	this->init_window();
	// Create pointer to Logger instance 
	this->log = new Log();
	// Create pointer to Renderer instance
	this->renderer = new Renderer(this->log, window);
	// Create poiner to Clock instance
	this->clock = new Clock();

	// Init initial assets
	b_AssetManager::InitAssets(this->log);

	this->init_io();
};

void Engine::init_window()
{
	if (!glfwInit())
	{
		printf("Could not init GLFW\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// Set MSAA parametres
	glfwWindowHint(GLFW_SAMPLES, 4);

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

void Engine::init_io()
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

void Engine::toggleGameMode()
{
	is_game_mode = !is_game_mode;
	if (is_game_mode)
		/* Hide and lock cursor */
		glfwSetInputMode(
			this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED
		);
	else
		/* Set cursor visible */
		glfwSetInputMode(
			this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL
		);
};

void Engine::takeScreenshot()
{
	int width, height;
    glfwGetFramebufferSize(this->window, &width, &height);

	// !!! Creates "Segmentation fault" error when vidmode != (1920, 1020) or (WIN_WIDTH, WIN_HEIGHT)
	if (   (width  == WIN_WIDTH  || width  == 1920)
		&& (height == WIN_HEIGHT || height == 1020))
	{
		unsigned char* pixels = new unsigned char[width * height * 3];

		// Bind main framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// Read pixels from main framebuffer
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		// Get filename
		std::string file_name{
			fs::path(FS_SCREENSHOTS_PATH) / ("Screenshot " + b_Utils::current_time_s() + ".png")
		};
		
		// Write pixels to image
		stbi_flip_vertically_on_write(1);
		int write_status = stbi_write_png(
			file_name.c_str(), width, height,
			3, pixels, width * 3
		);
		stbi_flip_vertically_on_write(0);
		if (!write_status)
			this->log->logf("[WARNING] Engine - Could not take screenshot\n");

		delete[] pixels;
	}
	else
	{
		printf ("Engine - Sorry, cannot take screenshot =(\n");
	}
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
bool Engine::isGameMode() const
{
	return this->is_game_mode;
};

/* ================== Update Methods ================== */
void Engine::OpenGameLoop()
{
	this->clock->start();
	glfwPollEvents();
	this->input->resetMouse();
	this->input->updateMouse(this->window);
};
void Engine::CloseGameLoop()
{
	this->clock->end();
};
/* ==================================================== */

Engine::~Engine()
{
	glfwDestroyWindow(this->window);
	glfwTerminate();
	
	// Release initial assets
	b_AssetManager::ReleaseAssets();
	
	delete this->renderer;
	delete this->clock;
	delete this->log;
	delete this->input;
	printf("Bye :)\n");
};