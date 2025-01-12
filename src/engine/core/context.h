#pragma once

#include "../util/deps.h"

#define b_CheckError() b_Context::CheckError(__FILE__, __LINE__) 

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
	GLenum CheckError(const char *file, int line);
};