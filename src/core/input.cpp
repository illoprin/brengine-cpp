#include "input.h"

#include "engine.h"

Input::Input()
{
	this->mouse.m_last_x = 0.0;	this->mouse.m_last_y = 0.0;
};

Input::~Input()
{
	LOG_MSG("IO released");
};

void Input::resetMouse()
{
	this->mouse.m_delta_x = 0;
	this->mouse.m_delta_y = 0;
}

void Input::updateMouse()
{
	double mx, my;
	glfwGetCursorPos(b_Engine::getWindow(), &mx, &my);
	this->mouse.m_x = mx; this->mouse.m_y = my;
	this->mouse.m_delta_x = this->mouse.m_x - this->mouse.m_last_x;
	this->mouse.m_delta_y = this->mouse.m_y - this->mouse.m_last_y;
	this->mouse.m_last_x = mx; this->mouse.m_last_y = my;
};

void Input::setKeyCallback(b_UserKeyCallback func)
{
	this->u_kc = func;
	LOG_MSG("IO User key callback function 0x%X attached", func);
};
void Input::setMouseButtonCallback(b_UserMouseButtonCallback func)
{
	this->u_mbc = func;
	LOG_MSG("IO User key callback function 0x%X attached", func);
};
void Input::setMouseMotionCallback(b_UserMouseMotionCallback func)
{
	this->u_mmc = func;
	LOG_MSG("IO User key callback function 0x%X attached", func);
};
void Input::setCharacterCallback(b_UserCharacterCallback func)
{
	this->u_cc = func;
	LOG_MSG("IO User key callback function 0x%X attached", func);
};

b_UserKeyCallback Input::getKeyCallback() const
{
	return this->u_kc;
};
b_UserMouseButtonCallback Input::getMouseButtonCallback() const
{
	return this->u_mbc;
};
b_UserMouseMotionCallback Input::getMouseMotionCallback() const
{
	return this->u_mmc;
};
b_UserCharacterCallback Input::getCharacterCallback() const
{
	return this->u_cc;
};

const Mouse& Input::getMouse()
{
	return this->mouse;
};