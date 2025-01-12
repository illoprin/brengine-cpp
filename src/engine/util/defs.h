#ifndef B_DEFS
#define B_DEFS

typedef void (*b_UserKeyCallback)(int key, int action, int mods);
typedef void (*b_UserMouseButtonCallback)(int button, int action);
typedef void (*b_UserMouseMotionCallback)(double x, double y);
typedef void (*b_UserCharacterCallback)(int scancode);
typedef void (*b_UserUpdateFunc)(float time, float deltaTime);

#define FS_TEMP_PATH "tmp"
#define FS_SCREENSHOTS_PATH "screenshots"
#define FS_LEVELS_PATH "levels"
#define FS_ASSETS_PATH "assets"
#define FS_LOG_PATH "log"

#define WIN_WIDTH 1280
#define WIN_HEIGHT 720
#define WIN_TITLE "Brengine 3D Alpha"

#define MAX_PATH 256
#define MAX_FILENAME 64
#define MAX_FILE_LINE 128

// GUI
enum GUIItemType
{
	GUI_SHAPE = 0,
	GUI_IMAGE,
	GUI_TEXT,
};
#define GUI_MAX_STRING_LEN 256

// Render
enum RenderMode
{
	RENDER_TEXTURED = 0,
	RENDER_NORMAL,
	RENDER_WIRE,
	RENDER_DEPTH,
};
// #ff17c9 - is fully alpha color
#define COLOR_NULL glm::vec3(1.0f, 0.09f, 0.79f)
// Handled by 'BaseMesh'
extern uint16_t b_draw_calls;
// Handled by 'BaseMesh'
extern uint16_t b_total_vertices;
// Handled by 'Renderer'
extern RenderMode b_render_mode;

// World
#define DIR_UP glm::vec3(0, 1, 0)
#define DIR_RIGHT glm::vec3(1, 0, 0)
#define DIR_FORWARD glm::vec3(0, 0, 1)

// Entity
enum EntityType {
	ENTITY_NULL = 0,
	ENTITY_STANDART,
	ENTITY_ON_LEVEL,
};

// Game
enum GameState {
	GS_NONE = 0,
	GS_MAIN_MENU = 0,
	GS_LEVEL,
	GS_PAUSED,
};

// Camera
#define CAM_FOV 90.f
#define CAM_NEAR .01f
#define CAM_FAR 100.f

// Player
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

// Level
#define LVL_SCALING 1.f

// Game data
#define BGD_FIELD_EOF "NULL"
#define BGD_PALETTE_TILE_SIZE (10)

#endif