#pragma once

#include "utils/deps.h"
#include "log.h"

struct Mouse
{
	// m - Mouse
	double m_last_x, m_last_y;
	double m_x, m_y;
	double m_delta_x, m_delta_y;
};

typedef void (*b_UserKeyCallback)(int key, int action, int mods);
typedef void (*b_UserMouseButtonCallback)(int button, int action);
typedef void (*b_UserMouseMotionCallback)(double x, double y);
typedef void (*b_UserCharacterCallback)(int scancode);

class Input
{
public:
	Input(Log* logger);
	~Input();

	// Delete copy operators
	Input(Input&) = delete;
	Input& operator=(Input&) = delete;
	Input& operator=(const Input&) = delete;

	void setKeyCallback(b_UserKeyCallback func);
	void setMouseButtonCallback(b_UserMouseButtonCallback func);
	void setMouseMotionCallback(b_UserMouseMotionCallback func);
	void setCharacterCallback(b_UserCharacterCallback func);

	b_UserKeyCallback getKeyCallback() const;
	b_UserMouseButtonCallback getMouseButtonCallback() const;
	b_UserMouseMotionCallback getMouseMotionCallback() const;
	b_UserCharacterCallback getCharacterCallback() const;

	Mouse* getMouse();
	void updateMouse(double x, double y);
private:
	Log* log;

	// u - User functions
	b_UserKeyCallback u_kc = nullptr;
	b_UserMouseButtonCallback u_mbc = nullptr;
	b_UserMouseMotionCallback u_mmc = nullptr;
	b_UserCharacterCallback u_cc = nullptr;

	Mouse mouse;
};