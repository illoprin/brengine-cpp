#pragma once

#include "../utils/deps.h"


struct ModelVertex {
	float vx, vy, vz, tu, tv, nx, ny, nz;
};

struct Triangle {
	struct ModelVertex vertex[3];
};

typedef std::vector<Triangle> ModelTriangles;