#ifndef _B_ENGINE_H_
#define _B_ENGINE_H_

#include "../util/deps.h"
#include "log.h"
#include "clock.h"
#include "input.h"
#include "camera.h"

#include "../gui/gui_scene.h"
#include "../gui/debug_gui.h"

#include "../render/master_renderer.h"
#include "../render/gui_renderer.h"
#include "default_assets.h"
#include "context.h"

// Fuck classes, I use namespaces

extern Log* b_log;
namespace b_Engine
{
	void Init();
	void Release();
	
	void OpenGameLoop();
	void UpdateDebugGUI(Camera*);
	void RenderDebugGUI(GUIRenderer&);
	void CloseGameLoop();

	void ToggleGameMode();
	void TakeScreenshot();
	
	void SetEngineUserPointer(void*);
	void* GetEngineUserPointer();

	GLFWwindow* getWindow();
	MasterRenderer* getRenderer();
	Clock* getClock();
	Input* getIO();
	bool isGameMode();
	bool isRunning();

	glm::ivec2 getVidMode();
	float getAspect();
};

#endif