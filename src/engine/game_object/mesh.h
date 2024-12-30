#pragma once

#include "../utils/deps.h"

namespace b_Model
{
	struct ModelVertex {
		float vx, vy, vz, tu, tv, nx, ny, nz;
	};

	struct Triangle {
		struct ModelVertex vertex[3];
	};

	typedef std::vector<Triangle> ModelTriangles;

	void loadFromOBJ(
		std::string filename, std::string& name, std::vector<Triangle>& tris
	);

};