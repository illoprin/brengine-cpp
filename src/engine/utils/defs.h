#ifndef B_DEFS
#define B_DEFS

#define FS_TEMP_PATH "tmp"
#define FS_SCREENSHOTS_PATH "screenshots"
#define FS_ASSETS_PATH "assets"
#define FS_LOG_PATH "log"

#define WIN_WIDTH 1600
#define WIN_HEIGHT 900
#define WIN_TITLE "Brengine 3D Alpha"

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

enum RenderMode
{
	RENDER_TEXTURED = 0,
	RENDER_NORMAL,
	RENDER_WIRE,
	RENDER_DEPTH,
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
#define PLAYER_SENSITIVITY 10.8f
#define PLAYER_SPEED 3.f
#define PLAYER_SPEED_MODIFER 1.78f // Sprint ratio

/* 	Enum design:
 *		PL - name of entity
 *		M - type of action (movent current ex)
 *		FORWARD - direction
 */
enum PlayerActionList {
	PL_M_FORWARD = 0,
	PL_M_BACKWARD,
	PL_M_LEFT,
	PL_M_RIGHT,
	PL_M_MODIFY,
};

#endif