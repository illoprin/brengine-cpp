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

std::string b_Files::__getPath(
	const std::string& filename, const std::string& path)
{
	return {fs::path(path) / filename};
};