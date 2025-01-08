#pragma once

#include "utils/deps.h"
#include "camera.h"
#include "engine.h"

class EditorController
{
public:
	EditorController();
	~EditorController();

	void update();

	Camera* getCamera() const;
private:
	Camera* camera;
	int states[5];

	void update_states();
	void handle_key_events();
	void handle_mouse_events();
};