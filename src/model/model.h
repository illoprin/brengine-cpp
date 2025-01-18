#pragma once

#include <util/math_includes.h>
#include <util/c_includes.h>
#include <util/cpp_includes.h>

#include <core/log.h>
#include <core/files.h>

namespace b_Model
{
	struct ModelVertex
	{
		float vx, vy, vz, tu, tv, nx, ny, nz;
	};

	struct Triangle
	{
		struct ModelVertex vertex[3];
	};

	struct SimpleVertex2D
	{
		glm::vec2 position;
		glm::vec2 texcoord;
	}; 

	typedef std::vector<Triangle> ModelTriangles;

	void ModelFromOBJ(
		std::string filename, std::string& name,
		std::vector<Triangle>& tris
	);

	void printTriangles(ModelTriangles& tris);
};