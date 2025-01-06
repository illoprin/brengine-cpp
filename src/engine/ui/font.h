#pragma once

#include "../utils/deps.h"

namespace b_Font
{
	// ASCII English symbols index range
	const char SYMBOLS_START_INDEX = 33;
	const char SYMBOLS_END_INDEX = 123;

	void BufferFromTTF(
		const char*      fp,
		unsigned char**  buffer,
		size_t*          size
	);
};