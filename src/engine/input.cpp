#include "input.h"

Input::Input(Log* logger)
{
	this->log = logger;
	this->mouse.m_last_x = 0.0;	this->mouse.m_last_y = 0.0;
};

Input::~Input()
{
	printf("IO: Released\n");
};

void Input::updateMouse(double x, double y)
{
	this->mouse.m_x = x; this->mouse.m_y = y;
	this->mouse.m_delta_x = this->mouse.m_x - this->mouse.m_last_x;
	this->mouse.m_delta_y = this->mouse.m_y - this->mouse.m_last_y;
	this->mouse.m_last_x = x; this->mouse.m_last_y = y;
};

void Input::setKeyCallback(b_UserKeyCallback func)
{
	this->u_kc = func;
	this->log->logf("[INFO] IO: User key callback function 0x%X attached\n", func);
};
void Input::setMouseButtonCallback(b_UserMouseButtonCallback func)
{
	this->u_mbc = func;
	this->log->logf("[INFO] IO: User mouse callback function 0x%X attached\n", func);
};
void Input::setMouseMotionCallback(b_UserMouseMotionCallback func)
{
	this->u_mmc = func;
	this->log->logf("[INFO] IO: User cursor motion callback function 0x%X attached\n", func);
};
void Input::setCharacterCallback(b_UserCharacterCallback func)
{
	this->u_cc = func;
	this->log->logf("[INFO] IO: User keyboard input callback function 0x%X attached\n", func);
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

Mouse* Input::getMouse()
{
	return &(this->mouse);
};