#pragma once

#include "../util/deps.h"

namespace b_Context
{
	void Init();
	void Prepare();
	void Bind();
	void Clear();
	void EnableFaceCulling();
	void DisableFaceCulling();
	void EnableDepthTest();
	void DisableDepthTest();
};