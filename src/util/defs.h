#ifndef B_DEFS
#define B_DEFS

#include <glm/glm.hpp>

typedef void (*b_UserKeyCallback)(int key, int action, int mods);
typedef void (*b_UserMouseButtonCallback)(int button, int action);
typedef void (*b_UserMouseMotionCallback)(double x, double y);
typedef void (*b_UserCharacterCallback)(int scancode);
typedef void (*b_UserUpdateFunc)(float time, float deltaTime);

typedef unsigned char uch;

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define WIN_TITLE "Brengine 3D Alpha"

// GUI
enum GUIItemType
{
	GUI_SHAPE = 0,
	GUI_IMAGE,
	GUI_TEXT,
};
#define GUI_MAX_STRING_LEN 256

// #ff17c9 - is fully alpha color
#define COLOR_NULL glm::vec3(1.0f, 0.09f, 0.79f)


#endif