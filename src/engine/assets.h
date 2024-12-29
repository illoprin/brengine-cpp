#ifndef B_ASSETS
#define B_ASSETS

#include "utils/deps.h"
#include "game_object/mesh.h"

namespace b_AssetManager
{
	// MD - mesh data
	std::vector<Triangle> QuadTriangles(float size);
	std::vector<Triangle> CubeTriangles(float size);
};

#endif