#ifndef _B_ENGINE_H_
#define _B_ENGINE_H_

#include "utils/deps.h"
#include "log.h"
#include "clock.h"
#include "input.h"

#include "ui/gui_scene.h"
#include "ui/debug_gui.h"
#include "scene_3d.h"

#include "render/renderer.h"
#include "assets.h"

// Fuck classes, I use namespaces

extern Log* b_log;
namespace b_Engine
{
	void Init();
	void Release();
	void SetScene(Scene3D*);
	void SetUI(b_GUI::GUIScene*);
	void Run();

	void ToggleGameMode();
	void TakeScreenshot();

	void SetUserUpdateFunction(b_UserUpdateFunc);

	void SetEngineUserPointer(void*);
	void* GetEngineUserPointer();

	GLFWwindow* getWindow();
	Renderer* getRenderer();
	Clock* getClock();
	Input* getIO();
	bool isGameMode();

	glm::ivec2 getVidMode();
	float getAspect();
};

#endif