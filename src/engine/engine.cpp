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

// --- Some internal functions declarations
static void game_loop_open();
static void game_loop_update();
static void game_loop_render();
static void game_loop_close();
static void init_window();
static void init_io();

// Create unique pointers to single classes (bu_ = brengine unique)
static std::unique_ptr<Log> bu_log = std::make_unique<Log>();
static std::unique_ptr<Clock> bu_clock;
static std::unique_ptr<Renderer> bu_renderer;

// Base
Log* b_log = bu_log.get(); // Global Log ptr
static Clock* b_clock;
static Renderer* b_renderer;
static Input* b_input;
static GLFWwindow* b_window;

// User
static void* usr_ptr = nullptr;
static b_UserUpdateFunc usr_update_func = nullptr;
static Scene3D* usr_scene = nullptr;
static GUIScene* usr_gui = nullptr;

// Engine static
static DebugGUI* s_debug;

// Window size
static glm::ivec2 w_vid_mode {WIN_WIDTH, WIN_HEIGHT};
float w_aspect = 1.f;

// Game
bool is_game_mode = false;

/* ================ Engine - Public Interface ================ */

void b_Engine::Init()
{
	b_Files::InitFilesystem();

	init_window();

	// Init renderer after window init
	bu_clock = std::make_unique<Clock>();
	b_clock = bu_clock.get();
	bu_renderer = std::make_unique<Renderer>();
	b_renderer = bu_renderer.get();
	b_AssetManager::InitAssets();
	init_io();

	s_debug = new DebugGUI;
};

void b_Engine::Release()
{
	b_log->logf("[INFO] Engine - Closing...\n");
	glfwDestroyWindow(b_window);
	glfwTerminate();
	
	// Release initial assets
	b_AssetManager::ReleaseAssets();
	
	delete b_input;
};

void b_Engine::SetScene(Scene3D* s)
{
	usr_scene = s;
};

void b_Engine::SetUI(b_GUI::GUIScene* s)
{
	usr_gui = s;
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

void b_Engine::Run()
{
	while(!glfwWindowShouldClose(b_window))
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
Renderer* b_Engine::getRenderer()
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
}

// --- Setters
void b_Engine::SetUserUpdateFunction(b_UserUpdateFunc func)
{
	usr_update_func = func;
};
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

/* ================== Update Methods ================== */

static void game_loop_update()
{
	if (usr_update_func != nullptr)
	{
		usr_update_func(
			(float) b_clock->getTime(),
			(float) b_clock->getDeltaTime()
		);		
	}

	s_debug->updateFPSText();
	if (usr_scene != nullptr)
	{
		s_debug->updatePlayerPosText(usr_scene->getCameraMain());
		usr_scene->update();
	}
	if (usr_gui != nullptr) 
	{
		// GUI scene update
	};

	b_renderer->Update();
};
static void game_loop_render()
{
	b_renderer->ClearCanvas();
	if (usr_scene != nullptr)
		b_renderer->RenderScene(*usr_scene);
	b_renderer->RenderUI(s_debug->s_ui_debug);
	if (usr_gui != nullptr)
		b_renderer->RenderUI(*usr_gui);
	b_renderer->Flush();
};
static void game_loop_open()
{
	b_clock->start();

	glfwPollEvents();
	b_input->resetMouse();
	b_input->updateMouse();
};
static void game_loop_close()
{
	b_clock->end();
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
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		
		if (key == GLFW_KEY_TAB) b_Engine::ToggleGameMode();

		if (key == GLFW_KEY_F5) b_renderer->switchRenderMode();
		if (key == GLFW_KEY_F2) b_Engine::TakeScreenshot();
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
	b_renderer->FramebufferSizeChange();
    glViewport(0, 0, width, height);
	game_loop_render();
}
/* ==================================================== */
