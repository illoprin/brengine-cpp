#include "clock.h"

Clock::Clock()
{
	this->time = 0;
	this->delta_time = 0;
	this->fps = 0;
	this->last_time = 0;
};

double Clock::getDeltaTime()
{
	return this->delta_time;
};

double Clock::getTime()
{
	return this->time;
};

void Clock::start()
{
	this->time = glfwGetTime();
};

void Clock::end()
{
	this->delta_time = this->time - this->last_time;
	this->last_time = this->time;
	this->fps = this->delta_time != 0 && this->delta_time < 1.0 ? 1.0 / this->delta_time : 0.0;
	//if ((int)this->time % 3 == 0)
	//	printf("TIME: %2.f | FPS: %2.f\n", this->time, this->fps);
};

Clock::~Clock()
{
	printf("Clock - Released\n");
};