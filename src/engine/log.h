#pragma once

#include "utils/deps.h"

class Log
{
public:
	Log(const char* path);

	// Delete copy operators
	Log(Log&) = delete;
	Log& operator=(Log&) = delete;
	Log& operator=(const Log&) = delete;

	int logf(const char* format, ...);

	~Log();
private:
	FILE* file;
};