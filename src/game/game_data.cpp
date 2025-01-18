#include "game_data.h"

using namespace b_Game;

void b_Game::GameDataFromBGD(
	const std::string file_name, GameData& gd
)
{
	gd.file_name = file_name;
	std::ifstream file{GAME_DATA_FILE_PATH(file_name)};

	if (!file.is_open())
	{
		LOG_ERR("Could not open file %s\n", file_name.c_str());
		abort();
		return;
	};

	std::string line;
	unsigned line_index = 0;
	BGDSeekType type = BGDSeekType::BGD_NOTHING;
	while (std::getline(file, line))
	{
		++line_index;
		std::istringstream sline{line};
		// First line - game name, read it
		if (line_index == 1)
		{
			std::string buf;
			// Skip comment
			sline >> buf;
			// Game name
			sline >> buf;

			gd.game_name = buf.substr(0, buf.find('\n')-1);
		}
		else
		{
			// Skip comments or empte lines
			if (line[0] == '#' || line.empty() || line[0] == '\n') continue;
			std::string token;
			sline >> token;
			// Resolve 'using' directives
			if (token.substr(0, 5) == "using")
			{
				std::string buf;

				// buf is - filename.ext
				sline >> buf;
				std::string filename, extension;
				b_Utils::SplitFilename(buf, filename, extension);
				if (extension == "bmp")
				{
					gd.textures.push_back(buf);
					continue;

				}
				else if (extension == "obj" || extension == "fbx")
				{
					gd.models.push_back(buf);
					continue;
				}
				else if (extension == "blf")
				{
					gd.levels.push_back(filename);
					continue;
				}
				LOG_WAR("Cannot resolve at line %d\n\t%s\n", line_index, line.c_str());
			}

			if (token.substr(0, 16) == "[ENT_APPEARANCE]")
			{
				type = BGD_ENT_APPEARANCE;
				continue;
			}
			else if (token.substr(0, 16) == "[LVL_APPEARANCE]")
			{
				type = BGD_LVL_APPEARANCE;
				continue;
			}
			else if (token.substr(0, 9) == "[PALETTE]")
			{
				type = BGD_PALLETE;	
				continue;
			}
			

			uch r, g, b; // For palette
			LevelAppearanceData la;
			switch (type)
			{
				// -- PALETTE
				case BGD_PALLETE:
					r = (uch)atoi(token.c_str());
					sline >> token;

					g = (uch)atoi(token.c_str());
					sline >> token;
					
					b = (uch)atoi(token.c_str());
					gd.pallete.push_back({r, g, b});
					continue;
				break;
				// -- LEVEL APPEARANCE
				case BGD_LVL_APPEARANCE:
					// 1. Texture index
					la.itexture = atoi(token.c_str());
					// 2. Palette color index
					sline >> token;
					la.icolor = atoi(token.c_str());
					// 3. Glow intencity 
					sline >> token;
					la.glow_intensity = atof(token.c_str());

					gd.lvl_appearance.push_back(la);
				break;
				default:
					printf("Reading nothing\n");
				break;
			}
		}
	};
	file.close();
};

void b_Game::GameData::print()
{
	printf("Game name: %s\n", game_name.c_str());
			
	puts("Levels:\n");
	for (const auto& l : levels)
	{
		printf("\tUsing level %s\n", l.c_str());
	}

	puts("Textures:\n");
	for (const std::string& s : textures)
	{
		printf("\tUsing texture %s\n", s.c_str());
	};

	puts("Models:\n");
	for (const std::string& s : models)
	{
		printf("\tUsing model %s\n", s.c_str());
	};

	puts("Level appearance:\n");
	for (const b_Game::LevelAppearanceData& la : lvl_appearance)
	{
		printf("\tTexIndex: %u ColorIndex: %u GlowIntensity: %.2f\n",
			la.itexture, la.icolor, la.glow_intensity);
	};

	puts("Palettes:\n");
	for (const glm::ivec3& col : pallete)
	{
		printf("\tColor R: %d %d %d\n", col.r, col.g, col.b);
	};
};