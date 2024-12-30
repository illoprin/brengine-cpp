#pragma once

#include "utils/deps.h"
#include "utils/utils.h"

class Log
{
public:
	Log();

	// Delete copy operators
	Log(Log&) = delete;
	Log& operator=(Log&) = delete;
	Log& operator=(const Log&) = delete;

	int logf(const char* format, ...);

	~Log();
private:
	FILE* file;
};