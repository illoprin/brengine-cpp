#pragma once

#include <util/deps.h>
#include <util/utils.h>
#include <core/files.h>
#include <core/log.h>

namespace b_Level
{

	struct Sector
	{
		// Wall start index
		uint16_t wall_start;
		// Number of walls
		uint16_t wall_num;
		float floor_height;
		float ceiling_height;
		// lvl_appearance index for floor
		uint16_t iafloor;
		// lvl_appearance index for ceiling
		uint16_t iaceil;
	};

	struct Wall
	{
		// Vertex 1 index
		uint16_t v1;
		// Vertex 2 index
		uint16_t v2;
		// Sector index which wall is portal to
		uint16_t portal;

		// lvl_appearance index
		uint16_t iappearance;
	};


	struct LevelData
	{
		std::string name = "nothing";
		std::vector<glm::vec2> verts;
		std::vector<Sector> sectors;
		std::vector<Wall> walls;

		void print();
		size_t get_size();
		const bool empty();
		void clear();
	};

	enum BLFSeekType{
		BLF_NOTHING = 0,
		BLF_VERTS,
		BLF_WALLS,
		BLF_SECTORS,
	};

	void FromBLF(std::string, LevelData&);
};