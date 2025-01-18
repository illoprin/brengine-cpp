#include "level_data.h"

using namespace b_Level;

void LevelData::print()
{
	printf("\nLevel %s\nVertices:\n", this->name.c_str());
	for (glm::vec2& vert : this->verts)
		printf("\tvX: %.1f vY: %.1f\n", vert.x, vert.y);
	printf("Walls:\n");
	for (const auto& p : walls)
	{
		const Wall& w = p.second;
		printf("\tIndex: %u V1: %u V2: %u Portal: %u iA: %u\n", p.first, w.v1, w.v2, w.portal, w.iappearance);
	}
	printf("Sectors:\n");
	for (const auto& p : sectors)
	{
		const Sector& s = p.second;
		printf("\tIndex: %u Wall Start Index: %u Num Walls: %u Floor height: %.2f Ceiling height: %.2f iACeil: %u, iAFloor: %u\n",
			p.first, s.wall_start, s.wall_num, s.floor_height, s.ceiling_height, s.iaceil, s.iafloor);
	}
	printf("Size of level is: %lu bytes\n\n", get_size());
}

size_t LevelData::get_size()
{
	return name.size() + game_name.size()
		+ verts.size() * sizeof(glm::vec2)
		+ sectors.size() * sizeof(Sector) + sectors.size() * sizeof(uint16_t)
		+ walls.size() * sizeof(Wall) + walls.size() * sizeof(uint16_t);
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
	std::string file_path{
		fs::path(FS_ASSETS_PATH) / FS_LEVELS_PATH / (file + ".blf")
	};

	std::ifstream src{file_path};

	if (!src.is_open())
	{
		fprintf(stderr, "b_Level::FromBLF - Could not open file with path %s\n", file_path.c_str());
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
				b_Utils::file_name_and_ext_from_str(token, filename, ext);
				if (ext == "bgd")
				{
					printf("b_Level::FromBLF - Level %s related to game %s\n", ld.name.c_str(), filename.c_str());
					ld.game_name = filename;
					continue;
				};
				printf("b_Level::FromBLF - Warning - Could not resolve at line %d\n\t%s\n", line_index, line.c_str());
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
			uint16_t index;
			Sector s; Wall w; glm::vec2 vt;
			switch (type)
			{
				case BLF_SECTORS:
					index = (uint16_t)atoi(token.c_str());
					s.wall_start = (uint16_t)atoi(tokens[0].c_str());
					s.wall_num = (uint16_t)atoi(tokens[1].c_str());
					s.floor_height = atof(tokens[2].c_str());
					s.ceiling_height = atof(tokens[3].c_str());
					if (tokens[4].substr(0, 4) == BGD_FIELD_EOF)
						s.iaceil = 0u;
					else
						s.iaceil = (uint16_t)atoi(tokens[4].c_str());
					if (tokens[6].substr(0, 4) == BGD_FIELD_EOF)
						s.iafloor = 0u;
					else
						s.iafloor = (uint16_t)atoi(tokens[5].c_str());
					ld.sectors.insert({ index, s });
					continue;
				break;
				case BLF_VERTS:
					vt.x = atof(token.c_str());
					vt.y = atof(tokens[0].c_str());
					ld.verts.push_back(vt);
					continue;
				break;
				case BLF_WALLS:
					index = (uint16_t)atoi(token.c_str());
					w.v1 = (uint16_t)atoi(tokens[0].c_str());
					w.v2 = (uint16_t)atoi(tokens[1].c_str());
					if (tokens[2].substr(0, 4) == BGD_FIELD_EOF)
						w.portal = 0u;
					else
						w.portal = (uint16_t)atoi(tokens[2].c_str());
					if (tokens[3].substr(0, 4) == BGD_FIELD_EOF)
						w.iappearance = 0u;
					else
						w.iappearance = (uint16_t)atoi(tokens[3].c_str());
					ld.walls.insert({ index, w });
					continue;
				break;
				default:
					printf("b_Level::FromBLF - Seeking nothing at line %d\n", line_index);
				break;
			};
			printf("b_Level::FromBLF - Warning - Could not resolve at line %d\n\t%s\n",
				line_index, line.c_str());
		}
	};
	// Close file
	src.close();

};