#pragma once

#include <util/gl_includes.h>
#include <core/log.h>

class Clock
{
public:
	Clock();
	~Clock();

	double getTime();
	double getDeltaTime();

	void start();
	void end();

	// Copy operator
	Clock(Clock& b) noexcept;
	// Equasion coppy opertaor delete
	Clock& operator=(Clock&) = delete;
private:
	double delta_time = 0;
	double time = 0;
	double last_time = 0;
	double fps = 0;
};