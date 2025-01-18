#pragma once

#include <util/c_includes.h>
#include <util/cpp_includes.h>
#include <core/files.h>

namespace b_Utils
{
	void ReadFileLines(const char* filepath, std::string& buffer);
	std::string TimeString();
	void SplitFilename(
		std::string src, std::string&, std::string&
	);
};

namespace b_ImageIO
{
	void GenBytes(std::vector<unsigned char>& bytes, unsigned width, unsigned height);

	// Flips image pixels by Y axis
	void FlipY(
		unsigned char* bytes,
		unsigned       components,
		unsigned       width,
		unsigned       height
	);
};

