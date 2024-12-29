#include "assets.h"

std::vector<Triangle> b_AssetManager::QuadTriangles(float size)
{
	std::vector<Triangle> tris{
		{
			{
				{size, size, 0.f, 1.f, 1.f, 0.f, 0.f, -1.f},
				{-size, -size, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f},
				{size, -size, 0.f, 1.f, 0.f, 0.f, 0.f, -1.f}
			}
		},
		{
			{
				{size, size, 0.f, 1.f, 1.f, 0.f, 0.f, -1.f},
				{-size, size, 0.f, 0.f, 1.f, 0.f, 0.f, -1.f},
				{-size, -size, 0.f, 0.f, 0.f, 0.f, 0.f, -1.f}
			}
		},
	};
	return tris;
};

std::vector<Triangle> b_AssetManager::CubeTriangles(float size)
{
	std::vector<Triangle> tris(12);
	return tris;
};