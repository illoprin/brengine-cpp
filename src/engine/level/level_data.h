#pragma once

#include "../utils/deps.h"

namespace b_Level
{

	struct Sector
	{
		unsigned wall_start;
		unsigned wall_num;
		float floor_height;
		float ceiling_height;
	};

	struct Wall
	{
		unsigned v1;
		unsigned v2;
		unsigned portal;
	};

	struct LevelData
	{
		std::string name = "nothing";
		std::vector<glm::vec2> verts;
		std::vector<Sector> sectors{Sector{}};
		std::vector<Wall> walls;

		void print();
		size_t get_size();
	};

	enum FileSeekType{
		NOTHING = 0,
		SEEK_VERTS,
		SEEK_WALLS,
		SEEK_SECTORS,
	};

	void FromBLF(std::string, LevelData&);
};