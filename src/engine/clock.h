#pragma once

#include "deps.h"

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
	double delta_time;
	double time;
	double last_time;
	double fps;
};