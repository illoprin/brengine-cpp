#include "level_data.h"

using namespace b_Level;

void LevelData::print()
{
	printf("\nLevel %s\nVertices:\n", this->name.c_str());
	for (glm::vec2& vert : this->verts)
		printf("\tvX: %.1f vY: %.1f\n", vert.x, vert.y);
	puts("Walls:\n");
	for (const Wall& w : walls)
	{
		printf("\tV1: %u V2: %u Portal: %u iA: %u\n", w.v1, w.v2, w.portal, w.iappearance);
	}
	puts("Sectors:\n");
	for (const Sector& s : sectors)
	{
		printf("\tWall Start Index: %u Num Walls: %u Floor height: %.2f Ceiling height: %.2f iACeil: %u, iAFloor: %u\n",
			s.wall_start, s.wall_num, s.floor_height, s.ceiling_height, s.iaceil, s.iafloor);
	}
	printf("Size of level is: %lu bytes\n\n", get_size());
}

size_t LevelData::get_size()
{
	return verts.size() * sizeof(glm::vec2)
		+ sectors.size() * sizeof(Sector)
		+ walls.size() * sizeof(Wall);
};

const bool LevelData::empty()
{
	return (verts.size()   > 1) ||
		   (sectors.size() > 1) ||
		   (walls.size()   > 1);
}

void LevelData::clear()
{
	verts.clear();
	sectors.clear();
	walls.clear();
};

void b_Level::FromBLF(std::string file, LevelData& ld)
{
	std::ifstream src{LEVEL_FILE_PATH(file)};

	if (!src.is_open())
	{
		LOG_ERR("Could not open level file with name %s", file.c_str());
		abort();
		return;
	};

	b_Level::BLFSeekType type = BLF_NOTHING;
	std::string line;
	unsigned line_index = 0;
	while(std::getline(src, line))
	{
		++line_index;
		std::istringstream sline{line};
		std::string token;
		sline >> token;

		// Resolve level name
		if (line_index == 1)
		{
			sline >> token;
			std::string level_name{token.substr(0, token.find('\n') - 1)};
			ld.name = level_name;
			continue;
		}
		else
		{
			// Skip comments and empty lines
			if (line[0] == '\n' || line[0] == '#' || line.empty()) continue;
			// Resolve 'from' directive
			if (token.substr(0, 4) == "from")
			{
				sline >> token;
				std::string filename, ext;
				b_Utils::SplitFilename(token, filename, ext);
				if (ext == "bgd")
				{
					LOG_MSG("Level %s related to game %s", ld.name.c_str(), filename.c_str());
					continue;
				};
				LOG_WAR("Could not resolve at line %d\n\t%s", line_index, line.c_str());
			};
			// Resolve '[DATA]'
			if (token.substr(0, 8) == "[VERTS]")
			{
				type = BLF_VERTS;
				continue;
			}
			else if (token.substr(0, 8) == "[WALLS]")
			{
				type = BLF_WALLS;
				continue;
			}
			else if (token.substr(0, 10) == "[SECTORS]")
			{
				type = BLF_SECTORS;
				continue;
			}

			// Get all line space divided tokens
			std::string tmp;
			std::vector<std::string> tokens{};
			while (sline >> tmp) tokens.push_back(tmp);

			// Switch by seek type
			Sector s; Wall w; glm::vec2 vt;
			switch (type)
			{
				case BLF_VERTS:
					// LOG_MSG("Seeking verts at line %u", line_index);
					vt.x = atof(token.c_str());
					vt.y = atof(tokens[0].c_str());
					ld.verts.push_back(vt);
					continue;
				break;
				case BLF_WALLS:
					// LOG_MSG("Seeking walls at line %u", line_index);
					w.v1 = (uint16_t)atoi(token.c_str());
					w.v2 = (uint16_t)atoi(tokens[0].c_str());
					w.portal = (uint16_t)atoi(tokens[1].c_str());
					w.iappearance = (uint16_t)atoi(tokens[2].c_str());
					ld.walls.push_back(w);
					continue;
				break;
				case BLF_SECTORS:
					// LOG_MSG("Seeking sectors at line %u", line_index);
					s.wall_start = (uint16_t)atoi(token.c_str());
					s.wall_num = (uint16_t)atoi(tokens[0].c_str());
					s.floor_height = atof(tokens[1].c_str());
					s.ceiling_height = atof(tokens[2].c_str());
					s.iaceil = (uint16_t)atoi(tokens[3].c_str());
					s.iafloor = (uint16_t)atoi(tokens[4].c_str());
					ld.sectors.push_back(s);
					continue;
				break;
				default:
					LOG_MSG("Seeking nothing at line %u", line_index);
				break;
			};
			LOG_WAR("Could not resolve at line %u\n\t%s", line.c_str());
		}
	};
	// Close file
	src.close();

	LOG_MSG("Level %s loaded", ld.name.c_str());
};