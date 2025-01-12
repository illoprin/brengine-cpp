#pragma once

#include "../util/deps.h"
#include "log.h"

struct Mouse
{
	// m - Mouse
	double m_last_x, m_last_y;
	double m_x, m_y;
	double m_delta_x, m_delta_y;
};

class Input
{
public:
	Input();
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

	const Mouse& getMouse();
	void updateMouse();
	void resetMouse();
private:
	// u - User functions
	b_UserKeyCallback u_kc = nullptr;
	b_UserMouseButtonCallback u_mbc = nullptr;
	b_UserMouseMotionCallback u_mmc = nullptr;
	b_UserCharacterCallback u_cc = nullptr;

	Mouse mouse;
};