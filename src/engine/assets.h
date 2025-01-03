#ifndef B_ASSETS
#define B_ASSETS

#include "utils/deps.h"
#include "game_object/mesh.h"

namespace b_AssetManager
{
	b_Model::ModelTriangles QuadTriangles(float size);
	b_Model::ModelTriangles CubeTriangles(float size);
};

#endif