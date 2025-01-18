#ifndef _B_ENGINE_H_
#define _B_ENGINE_H_

#include <util/cpp_includes.h>
#include <core/log.h>
#include <core/clock.h>
#include <core/input.h>
#include <core/files.h>

#include <gui/gui_scene.h>
#include <gui/debug_gui.h>

#include <render/master_renderer.h>
#include <render/gui_renderer.h>
#include <core/default_assets.h>
#include <core/context.h>

// Fuck classes, I use namespaces

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