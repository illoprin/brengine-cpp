#pragma once

#include "utils/deps.h"
#include "camera.h"

class EditorController
{
public:
	EditorController();
	~EditorController();
private:
	Camera* camera;
};