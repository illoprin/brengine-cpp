#pragma once

#include "deps.h"

namespace b_Utils
{
	void read_file_lines(const char* filepath, std::string& buffer);
	std::string current_time_s();
	void file_name_and_ext_from_str(
		std::string src, std::string&, std::string&
	);
};

namespace b_ImageIO
{
	void GenBytes(std::vector<unsigned char>& bytes, unsigned width, unsigned height);

	void WriteBytes(
		std::vector<unsigned char>& byteData,
		int width, 
		int height, 
		int components, 
		const char* path
	);

	// Flips image pixels by Y axis
	void FlipY(
		unsigned char* bytes,
		unsigned       components,
		unsigned       width,
		unsigned       height
	);
};

// Utils namespace for service engine's filesystem (directories and etc)
namespace b_Files
{ 
	// Init engine's directories
	void InitFilesystem();

	std::vector<std::string> AllFilesByPathWithExtension(const char* path, const char* ext);
};