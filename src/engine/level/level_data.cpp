#include "level_data.h"

using namespace b_Level;

void LevelData::print()
{
	printf("\nLevel %s\nVertices:\n", this->name.c_str());
	for (glm::vec2& vert : this->verts)
		printf("\tvX: %.1f vY: %.1f\n", vert.x, vert.y);
	
	printf("Walls:\n");
	for (Wall& w : this->walls)
		printf("\tV1: %u V2: %u Portal: %u\n", w.v1, w.v2, w.portal);
	
	printf("Sectors:\n");
	for (unsigned i = 0; i < this->sectors.size(); i++)
	{
		Sector& s = this->sectors[i];
		printf("\tIndex: %u Wall Start Index: %u Num Walls: %u Floor height: %.2f Ceiling height: %.2f\n",
			i, s.wall_start, s.wall_num, s.floor_height, s.ceiling_height);
	}
	
	printf("Size of level is: %lu bytes\n", get_size());
}

size_t LevelData::get_size()
{
	return name.size() + verts.size() * sizeof(glm::vec2) + sectors.size() * sizeof(Sector) + walls.size() * sizeof(Wall);
};

void b_Level::FromBLF(std::string file, LevelData& ld)
{
	std::string file_path{
		fs::path(FS_ASSETS_PATH) / FS_LEVELS_PATH / (file + ".blf")
	};
	
	FILE* ld_src = fopen(file_path.c_str(), "r");
	if (!ld_src)
	{
		fprintf(stderr, "b_Level::FromBLF - Could not open %s\n", file_path.c_str());
		return;
	}
	
	char line[MAX_FILE_LINE];
	char* token = NULL;
	unsigned current_line = 0u;
	FileSeekType type = FileSeekType::NOTHING;
	while (fgets(line, MAX_FILE_LINE, ld_src) != NULL)
	{
		current_line++;

		token = strtok(line, " ");
		if (!strcmp(line, "\n") || !strcmp(line, "\0"))
			continue;
		if (current_line != 1u)
		{
			if (!strncmp(token, "#", 1))
				continue;
			else if (token && !strncmp(token, "[WALLS]", 7))
			{
				type = FileSeekType::SEEK_WALLS;
				continue;
			}
			else if (token && !strncmp(token, "[VERTS]", 7))
			{
				type = FileSeekType::SEEK_VERTS;
				continue;
			}
			else if (token && !strncmp(token, "[SECTORS]", 9))
			{
				type = FileSeekType::SEEK_SECTORS;
				continue;
			}
			
			float v_x, v_y; // Verts
			unsigned v1, v2, p; // Walls
			unsigned ws, wn; float fh, ch; // Sector
			Wall w; Sector s;
			switch (type)
			{
				case SEEK_VERTS:
					printf("b_Level::FromBLF - Seeking Vertices on line %d\n", current_line);
					v_x = atof(token);
					token = strtok(NULL, " ");
					v_y = atof(token);
					ld.verts.push_back(glm::vec2(v_x, v_y));
					break;
				case SEEK_WALLS:
					printf("b_Level::FromBLF - Seeking Walls on line %d\n", current_line);
					// Skip wall index
					token = strtok(NULL, " ");
					v1 = atoi(token);
					token = strtok(NULL, " ");
					v2 = atoi(token);
					token = strtok(NULL, " ");
					p = atoi(token);
					w.v1 = v1; w.v2 = v2; w.portal = p;
					ld.walls.push_back(w);
					break;
				case SEEK_SECTORS:
					printf("b_Level::FromBLF - Seeking Sectors on line %d\n", current_line);
					// Skip sector index
					token = strtok(NULL, " ");
					ws = atoi(token);
					token = strtok(NULL, " ");
					wn = atoi(token);
					token = strtok(NULL, " ");
					fh = atof(token);
					token = strtok(NULL, " ");
					ch = atof(token);
					s.wall_start = ws; s.wall_num = wn;
					s.floor_height = fh; s.ceiling_height = ch;
					ld.sectors.push_back(s);
					break;
				default:
					printf("b_Level::FromBLF - Seeking Nothing on line %d\n", current_line);
			};
		}
		else
		{
			std::string level_name{""};
			token = strtok(NULL, " ");

			while (token != nullptr)
			{
				level_name += token;
				level_name += " ";
				token = strtok(NULL, " ");
			}
			ld.name = level_name;
		};
	};

	// Closing file
	fclose(ld_src);
};