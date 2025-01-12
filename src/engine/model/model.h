#pragma once

#include "../util/deps.h"

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

	void loadFromOBJ(
		std::string filename, std::string& name,
		std::vector<Triangle>& tris
	);

	void printTriangles(ModelTriangles& tris);
};