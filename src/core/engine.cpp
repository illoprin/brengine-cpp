#include "engine.h"

#include "../mesh/base_mesh.h" // Stores information about draw calls number

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

// --- Some internal functions declarations
static void init_window();
static void init_io();

// Create unique pointers to single classes (bu_ = brengine unique)
static std::unique_ptr<Log> bu_log;
static std::unique_ptr<Clock> bu_clock;
static std::unique_ptr<MasterRenderer> bu_renderer;

// Base
Log* b_log = nullptr; // Global Log ptr
static Clock* b_clock;
static MasterRenderer* b_renderer;
static Input* b_input;
static GLFWwindow* b_window;

// User
static void* usr_ptr = nullptr;

// Window
static glm::ivec2 w_vid_mode {WIN_WIDTH, WIN_HEIGHT};
float w_aspect = 1.f;
bool is_running = false;

// Game
DebugGUI* ui_debug = nullptr;
bool is_game_mode = false;
bool is_info_visible = true;

/* ================ Engine - Public Interface ================ */

void b_Engine::Init()
{
	b_Files::InitFilesystem();

	init_window();

	// Init logger
	bu_log = std::make_unique<Log>();
	b_log = bu_log.get();

	// Init clock
	bu_clock = std::make_unique<Clock>();
	b_clock = bu_clock.get();

	// Init and prepare context
	b_Context::Init();
	b_Context::Prepare();
	
	// Enable VSync
	glfwSwapInterval(1); /* Creates window motion slowdown on X11 */

	// Init renderer after window init
	bu_renderer = std::make_unique<MasterRenderer>();
	b_renderer = bu_renderer.get();
	b_AssetManager::InitAssets();
	init_io();

	ui_debug = new DebugGUI;

	is_running = true;
};

void b_Engine::Release()
{
	b_log->logf("[INFO] Engine - Closing...\n");
	glfwDestroyWindow(b_window);
	glfwTerminate();
	
	// Release initial assets
	delete ui_debug;
	b_AssetManager::ReleaseAssets();
	
	delete b_input;
};

void b_Engine::ToggleGameMode()
{
	is_game_mode = !is_game_mode;
	if (is_game_mode)
		/* Hide and lock cursor */
		glfwSetInputMode(
			b_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED
		);
	else
		/* Set cursor visible */
		glfwSetInputMode(
			b_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL
		);
};

void b_Engine::TakeScreenshot()
{
	// !!! Creates "Segmentation fault" error when vidmode != (1920, 1020) or (WIN_WIDTH, WIN_HEIGHT)
	if (   (w_vid_mode.x  == WIN_WIDTH  || w_vid_mode.x == 1920)
		&& (w_vid_mode.y == WIN_HEIGHT || w_vid_mode.y == 1020))
	{
		unsigned char* pixels = (unsigned char*)calloc(w_vid_mode.x * w_vid_mode.y * 3, 1);

		// Bind main framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		// Read pixels from main framebuffer
		glReadPixels(0, 0, w_vid_mode.x, w_vid_mode.y, GL_RGB, GL_UNSIGNED_BYTE, pixels);

		// Get filename
		std::string file_name{
			fs::path(FS_SCREENSHOTS_PATH) / ("Screenshot " + b_Utils::current_time_s() + ".png")
		};
		
		// Write pixels to image
		stbi_flip_vertically_on_write(1);
		int write_status = stbi_write_png(
			file_name.c_str(), w_vid_mode.x, w_vid_mode.y,
			3, pixels, w_vid_mode.x * 3
		);
		if (!write_status)
			b_log->logf("[WARNING] Engine - Could not take screenshot\n");
		stbi_flip_vertically_on_write(0);

		free(pixels);
	}
	else
	{
		printf ("Engine - Sorry, cannot take screenshot =(\n");
	}

};

/* ================== Update Methods ================== */
void b_Engine::OpenGameLoop()
{
	// Start clock
	b_clock->start();

	// Handle events
	glfwPollEvents();
	
	// Update mouse
	b_input->resetMouse();
	b_input->updateMouse();
};
void b_Engine::UpdateDebugGUI(Camera* cam)
{
	ui_debug->updateFPSText();
	ui_debug->updateInfo(cam);
};
void b_Engine::RenderDebugGUI(GUIRenderer& r_gui)
{
	if (is_info_visible)
		r_gui.Render(ui_debug->s_ui_debug.getItems());
};
void b_Engine::CloseGameLoop()
{
	b_clock->end();
	glfwSwapBuffers(b_Engine::getWindow());
};
/* ==================================================== */

// --- Getters
glm::ivec2 b_Engine::getVidMode()
{
	return w_vid_mode;
};
GLFWwindow* b_Engine::getWindow()
{
	return b_window;
};
Clock* b_Engine::getClock()
{
	return b_clock;
};
MasterRenderer* b_Engine::getRenderer()
{
	return b_renderer;
};
Input* b_Engine::getIO()
{
	return b_input;
};
bool b_Engine::isGameMode()
{
	return is_game_mode;
};
float b_Engine::getAspect()
{
	return w_aspect;
};
bool b_Engine::isRunning()
{
	return (bool)(!glfwWindowShouldClose(b_window));
};

// --- Setters
void b_Engine::SetEngineUserPointer(void* ptr)
{
	usr_ptr = ptr;
};
void* b_Engine::GetEngineUserPointer()
{
	return usr_ptr;
};

/* =========================================================== */


static void init_window()
{
	if (glfwInit() != GLFW_TRUE)
	{
		printf("Could not init GLFW\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	b_window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_TITLE, NULL, NULL);

	if (!b_window)
	{
		printf("Could not create GLFW window\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	};

	// Init window-size-depend variables
	int w_w, w_h;
	glfwGetFramebufferSize(b_window, &w_w, &w_h);
	w_vid_mode.x = w_w;
	w_vid_mode.y = w_h;
	w_aspect = (float)w_w / (float)w_h;

	// Set window position in center of screen
	const GLFWvidmode* glfw_vidmode =
		glfwGetVideoMode(glfwGetPrimaryMonitor());
	int wx = glfw_vidmode->width / 2 - w_w / 2;
	int wy = glfw_vidmode->height / 2 - w_h / 2;
	glfwSetWindowPos(b_window, wx, wy);

	glfwFocusWindow(b_window);
};

static void init_io()
{
	b_input = new Input;

	glfwSetErrorCallback(errorCallback);
	glfwSetKeyCallback(b_window, keyCallback);
	glfwSetMouseButtonCallback(b_window, mouseButtonCallback);
	glfwSetCursorPosCallback(b_window, mouseMotionCallback);
	glfwSetFramebufferSizeCallback(b_window, framebufferSizeCallback);
};

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
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			is_running = false;
		}
		
		if (key == GLFW_KEY_TAB) b_Engine::ToggleGameMode();
		if (key == GLFW_KEY_F5) b_renderer->switchRenderMode();
		if (key == GLFW_KEY_F2) b_Engine::TakeScreenshot();
		if (key == GLFW_KEY_F1) is_info_visible = !is_info_visible;
	}
	// Execute user function
	b_UserKeyCallback user_func = b_input->getKeyCallback();
	if (user_func != nullptr) user_func(key, action, mods);
};

static void mouseMotionCallback(GLFWwindow* window, double x, double y)
{
	// Execute user function
	b_UserMouseMotionCallback user_func = b_input->getMouseMotionCallback();
	if (user_func != nullptr) user_func(x, y);
};

static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Execute user function
	b_UserMouseButtonCallback user_func = b_input->getMouseButtonCallback();
	if (user_func != nullptr) user_func(button, action);
};

static void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	w_vid_mode.x = width; w_vid_mode.y = height;
	w_aspect = (float) w_vid_mode.x / (float) w_vid_mode.y;

	b_log->logf("[INFO] Engine - window resized %d %d\n", width, height);
	b_renderer->WindowSizeChangeCallback();
}
/* ==================================================== */
