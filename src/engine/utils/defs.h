#ifndef B_DEFS
#define B_DEFS

#define WIN_WIDTH 1024
#define WIN_HEIGHT 768
#define WIN_TITLE "Game"

#define MAX_PATH 256
#define MAX_FILENAME 64
#define MAX_FILE_LINE 128

enum SceneTypes
{
	SCENE_FLAT = 1,
	SCENE_STANDART = 1 << 2,
	SCENE_UI = 1 << 3,
	SCENE_CANVAS = 1 << 4
};

// World
#define DIR_UP glm::vec3(0, 1, 0)
#define DIR_RIGHT glm::vec3(1, 0, 0)
#define DIR_FORWARD glm::vec3(0, 0, 1)

// Camera
#define CAM_FOV 90.f
#define CAM_NEAR .01f
#define CAM_FAR 100.f

// Player

#endif