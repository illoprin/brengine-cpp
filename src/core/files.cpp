#include "files.h"

void b_Files::InitFilesystem()
{
	if (!fs::is_directory(FS_ASSETS_PATH))
	{
		LOG_ERR("Could not init game without resources");
		exit(EXIT_FAILURE);
	};

	if (!fs::is_directory(FS_TEMP_PATH))
	{
		fs::create_directory(fs::path(FS_TEMP_PATH));
		LOG_MSG("Temp directory created");
	};

	if (!fs::is_directory(FS_SCREENSHOTS_PATH))
	{
		fs::create_directory(fs::path(FS_SCREENSHOTS_PATH));
		LOG_MSG("Screenshots directory created");
	};

	if (!fs::is_directory(FS_LOG_PATH))
	{
		fs::create_directory(FS_LOG_PATH);
		LOG_MSG("Log directory created");
	};
};

std::string b_Files::__getPath(PathType type, const char* filename)
{
	std::string full_path;

	switch (type)
	{
		case PathType::FILE_GAME_DATA:
			full_path = fs::path(FS_GAME_DATA_PATH) / std::string{filename};
		break;
		case PathType::FILE_LEVEL:
			full_path = fs::path(FS_LEVELS_PATH) / std::string{filename};
		break;
		case PathType::FILE_MODEL:
			full_path = fs::path(FS_MODELS_PATH) / std::string{filename};
		break;
		case PathType::FILE_SHADER:
			full_path = fs::path(FS_SHADERS_PATH) / std::string{filename};
		break;
		case PathType::FILE_SCREENSHOT:
			full_path = fs::path(FS_SCREENSHOTS_PATH) / std::string{filename};
		break;
	}

	return full_path;
};