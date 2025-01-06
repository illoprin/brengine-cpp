#include "assets.h"

void b_AssetManager::InitAssets()
{
	
};

b_Model::ModelTriangles b_AssetManager::QuadTriangles(float size)
{
	std::vector<b_Model::Triangle> tris{
		{
			{
				{size, -size, 0.f, 1.f, 0.f, 0.f, 0.f, -1.f},
				{-size, -size, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f},
				{size, size, 0.f, 1.f, 1.f, 0.f, 0.f, -1.f}
			}
		},
		{
			{
				{-size, -size, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f},
				{-size, size, 0.f, 0.f, 1.f, 0.f, 0.f, -1.f},
				{size, size, 0.f, 1.f, 1.f, 0.f, 0.f, -1.f}
			}
		},
	};
	return tris;
};

b_Model::ModelTriangles b_AssetManager::CubeTriangles(float size)
{
	std::vector<b_Model::Triangle> tris(12);
	return tris;
};