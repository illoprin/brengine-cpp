#include "engine.h"

using namespace b_GUI;

// --- Callbacks declaration
static void errorCallback (
	int, const char*
);
static void keyCallback (
	GLFWwindow*, int, int, int, int
);
static void mouseMotionCallback (
	GLFWwindow*, double, double
);
static void mouseButtonCallback (
	GLFWwindow*, int, int, int
);
static void framebufferSizeCallback (
	GLFWwindow*, int, int
);

// --- Game Loop functions declaration
static void game_loop_open();
static void game_loop_update();
static void game_loop_render();
static void game_loop_close();

// Base
static Log* log;
static Clock* e_clock;
static Renderer* renderer;
static Input* input;
static GLFWwindow* wnd;

// User
static void* usr_ptr = nullptr;
static b_UserUpdateFunc usr_update_func = nullptr;
static Scene3D* usr_scene = nullptr;
static GUIScene* usr_gui = nullptr;

// Window size
static glm::ivec2 vid_mode{WIN_WIDTH, WIN_HEIGHT};
float aspect = (float) WIN_HEIGHT / (float) WIN_WIDTH;

// Game
bool is_game_mode = false;

/* ================ Engine - Public Interface ================ */
void b_Engine::Init()
{
	b_Files::InitFilesystem();

	initWindow();

	// Create pointer to Logger instance 
	log = new Log();
	// Create pointer to Renderer instance
	renderer = new Renderer();
	// Create poiner to Clock instance
	e_clock = new Clock();

	initIO();
};
void b_Engine::Release()
{
	glfwDestroyWindow(wnd);
	glfwTerminate();
	
	// Release initial assets
	b_AssetManager::ReleaseAssets();
	
	delete renderer;
	delete e_clock;
	delete log;
	delete input;
	printf("Bye :)\n");
};

void b_Engine::ToggleGameMode()
{
	is_game_mode = !is_game_mode;
	if (is_game_mode)
		/* Hide and lock cursor */
		glfwSetInputMode(
			wnd, GLFW_CURSOR, GLFW_CURSOR_DISABLED
		);
	else
		/* Set cursor visible */
		glfwSetInputMode(
			wnd, GLFW_CURSOR, GLFW_CURSOR_NORMAL
		);
};
void b_Engine::TakeScreenshot()
{
	// !!! Creates "Segmentation fault" error when vidmode != (1920, 1020) or (WIN_WIDTH, WIN_HEIGHT)
	if (   (vid_mode.x  == WIN_WIDTH  || vid_mode.x == 1920)
		&& (vid_mode.y == WIN_HEIGHT || vid_mode.y == 1020))
	{
		unsigned char* pixels = (unsigned char*)calloc(vid_mode.x * vid_mode.y * 3, 1);

		// Bind main framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// Read pixels from main framebuffer
		glReadPixels(0, 0, vid_mode.x, vid_mode.y, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		// Get filename
		std::string file_name{
			fs::path(FS_SCREENSHOTS_PATH) / ("Screenshot " + b_Utils::current_time_s() + ".png")
		};
		
		// Write pixels to image
		stbi_flip_vertically_on_write(1);
		int write_status = stbi_write_png(
			file_name.c_str(), vid_mode.x, vid_mode.y,
			3, pixels, vid_mode.x * 3
		);
		if (!write_status)
			log->logf("[WARNING] Engine - Could not take screenshot\n");
		stbi_flip_vertically_on_write(0);

		free(pixels);
	}
	else
	{
		printf ("Engine - Sorry, cannot take screenshot =(\n");
	}

};

void b_Engine::Run()
{
	while(glfwWindowShouldClose(wnd))
	{
		game_loop_open();
		game_loop_update();
		game_loop_render();		
		game_loop_close();
	};
};

// --- Getters
glm::ivec2 b_Engine::getVidMode()
{
	return vid_mode;
};
GLFWwindow* b_Engine::getWindow()
{
	return wnd;
};
Log* b_Engine::getLogger()
{
	return log;
};
Clock* b_Engine::getClock()
{
	return e_clock;
};
Renderer* b_Engine::getRenderer()
{
	return renderer;
};
Input* b_Engine::getIO()
{
	return input;
};
bool b_Engine::isGameMode()
{
	return is_game_mode;
};

// --- Setters
inline void b_Engine::SetUserUpdateFunction(b_UserUpdateFunc func)
{
	usr_update_func = func;
};
inline void b_Engine::SetEngineUserPointer(void* ptr)
{
	usr_ptr = ptr;
};
inline void* b_Engine::GetEngineUserPointer()
{
	return usr_ptr;
};

/* =========================================================== */


static void initWindow()
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
	// Set window resizeable option
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	// Set MSAA parametres
	glfwWindowHint(GLFW_SAMPLES, 4);

	wnd = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);

	if (!wnd)
	{
		printf("Could not create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	};
	glfwFocusWindow(wnd);
};

static void initIO()
{
	input = new Input(log);
	glfwSetErrorCallback(errorCallback);

	glfwSetKeyCallback(wnd, keyCallback);
	glfwSetMouseButtonCallback(wnd, mouseButtonCallback);
	glfwSetCursorPosCallback(wnd, mouseMotionCallback);
	glfwSetFramebufferSizeCallback(wnd, framebufferSizeCallback);
};

/* ================== Update Methods ================== */

static void game_loop_update()
{
	if (usr_update_func != nullptr)
		usr_update_func(
			(float) e_clock->getTime(),
			(float) e_clock->getDeltaTime()
		);
	if (usr_scene != nullptr) usr_scene->update(vid_mode);
	if (usr_gui != nullptr) usr_gui->update(vid_mode);
};
static void game_loop_render()
{
	renderer->ClearCanvas();
	if (usr_scene != nullptr)
		renderer->RenderScene(*usr_scene);
	if (usr_gui != nullptr)
		renderer->RenderUI(*usr_gui);
	renderer->Flush();
};
static void game_loop_open()
{
	e_clock->start();

	glfwPollEvents();
	input->resetMouse();
	input->updateMouse(wnd);
};
static void game_loop_close()
{
	e_clock->end();
};
/* ==================================================== */


/* ================== Callbacks      ================== */
static void errorCallback(int error, const char* desc)
{
	fprintf(stderr, "Error: %s\n", desc);
};

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		if (key == GLFW_KEY_ESCAPE)
		{
			printf("Closing...\n");
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		
		if (key == GLFW_KEY_TAB) b_Engine::ToggleGameMode();

		if (key == GLFW_KEY_F5) renderer->switchRenderMode();
		if (key == GLFW_KEY_F2) b_Engine::TakeScreenshot();
	}
	// Execute user function
	b_UserKeyCallback user_func = input->getKeyCallback();
	if (user_func != nullptr) user_func(key, action, mods);
};

static void mouseMotionCallback(GLFWwindow* window, double x, double y)
{
	// Execute user function
	b_UserMouseMotionCallback user_func = input->getMouseMotionCallback();
	if (user_func != nullptr) user_func(x, y);
};

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Execute user function
	b_UserMouseButtonCallback user_func = input->getMouseButtonCallback();
	if (user_func != nullptr) user_func(button, action);
};

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	vid_mode.x = width; vid_mode.y = height;
	aspect = (float) vid_mode.x / (float) vid_mode.y;

	log->logf("[INFO] Engine - window resized %d %d\n", width, height);
	renderer->FramebufferSizeChange();
    glViewport(0, 0, width, height);
	game_loop_render();
}
/* ==================================================== */
