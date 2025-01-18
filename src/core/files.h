#pragma once

#include <util/cpp_includes.h>
#include <core/log.h>

#define FS_TEMP_PATH "tmp"
#define FS_ASSETS_PATH "assets"
#define FS_SCREENSHOTS_PATH "screenshots"
#define FS_LEVELS_PATH FS_ASSETS_PATH "/levels"
#define FS_TEXTURES_PATH FS_ASSETS_PATH "/textures"
#define FS_MODELS_PATH FS_ASSETS_PATH "/models"
#define FS_FONTS_PATH FS_ASSETS_PATH "/fonts"
#define FS_GAME_DATA_PATH FS_ASSETS_PATH "/game_data"
#define FS_SHADERS_PATH FS_ASSETS_PATH "/shaders"
#define FS_LOG_PATH FS_TEMP_PATH "/log"

enum {
	MAX_PATH = 320,
	MAX_FILENAME = 64,
	MAX_BUFFER = 256,
};

enum PathType {
	FILE_TEXTURE = 0,
	FILE_SHADER,
	FILE_MODEL,
	FILE_GAME_DATA,
	FILE_LEVEL,
	FILE_FONT,
	FILE_SCREENSHOT
};

// Namespace for service engine's filesystem (directories and etc)
namespace b_Files
{ 
	// Init engine's directories
	void InitFilesystem();

	std::string __getPath(const std::string& filename,
						  const std::string& path);
};

#define MODEL_FILE_PATH(name) b_Files::__getPath((name), FS_MODELS_PATH)

#define SHADER_FILE_PATH(name) b_Files::__getPath((name), FS_SHADERS_PATH)

#define GAME_DATA_FILE_PATH(name) b_Files::__getPath((name), FS_GAME_DATA_PATH)

#define LEVEL_FILE_PATH(name) b_Files::__getPath((name), FS_LEVELS_PATH)

#define TEXTURE_FILE_PATH(name) b_Files::__getPath((name), FS_TEXTURES_PATH)

#define FONT_FILE_PATH(name) b_Files::__getPath((name), FS_FONTS_PATH)

#define TMP_FILE_PATH(name) b_Files::__getPath((name), FS_TEMP_PATH)

// Get screenshot file full path
#define SCH_FILE_PATH(name) b_Files::__getPath(FILE_SCREENSHOT, (name))