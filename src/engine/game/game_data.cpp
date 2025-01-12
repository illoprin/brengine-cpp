#include "game_data.h"

using namespace b_Game;

void b_Game::GameDataFromBGD(
	const std::string file_name, GameData& gd
)
{
	gd.file_name = file_name;
	std::string file_path{"assets/game_data/" + file_name + ".bgd"}; 
	std::ifstream file{fs::path(file_path)};

	if (!file.is_open())
	{
		fprintf(stderr, "b_Game::GameDataFromBGD - Could not open file %s\n", file_name.c_str());
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
				b_Utils::file_name_and_ext_from_str(buf, filename, extension);
				if (extension == "bmp")
				{
					printf("b_Game::GameDataFromBGD - Line %d - Resolving image include %s\n", line_index, filename.c_str());
					// Skip 'as' keyword
					sline >> buf;
					// Get index of texture
					sline >> buf;
					uint16_t ti = atoi(buf.c_str()); // Get texture index
					gd.textures.insert({ ti, filename });
					continue;

				}
				else if (extension == "obj" || extension == "fbx")
				{
					printf("b_Game::GameDataFromBGD - Line %d - Resolving model include %s\n", line_index, filename.c_str());
					// Skip 'as' keyword
					sline >> buf;
					// Get index of model
					sline >> buf;
					uint16_t mi = atoi(buf.c_str()); // Get model index
					gd.models.insert({ mi , filename });
					continue;
				}
				else if (extension == "blf")
				{
					printf("b_Game::GameDataFromBGD - Line %d - Resolving level include %s\n", line_index, filename.c_str());
					gd.levels.push_back(filename);
					continue;
				}
				printf("b_Game::GameDataFromBGD - Warning - Cannot resolve at line %d\n\t%s\n", line_index, line.c_str());
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
			

			int index; float r, g, b; // For palette
			uint16_t la_texindex; uint16_t la_colindex; bool la_glowing; float la_glow;
			LevelAppearanceData la;
			switch (type)
			{
				case BGD_PALLETE:
					index = atoi(token.c_str());
					sline >> token;
					r = (float)atoi(token.c_str()) / 255.f;
					sline >> token;
					g = (float)atoi(token.c_str()) / 255.f;
					sline >> token;
					b = (float)atoi(token.c_str()) / 255.f;
					gd.palletes.insert({ index, {r, g, b} });
					continue;
				break;
				case BGD_LVL_APPEARANCE:
					index = atoi(token.c_str());
					// 1. Texture index
					sline >> token;
					if (token.substr(0,4) != BGD_FIELD_EOF)
						la_texindex = atoi(token.c_str());
					else
						la_texindex = 0u;
					// 2. Palette color index
					sline >> token;
					if (token.substr(0,4) != BGD_FIELD_EOF)
						la_colindex = atoi(token.c_str());
					else
						la_colindex = 0u;
					// 4. Is glowing
					sline >> token;
					la_glowing = (bool)atoi(token.c_str());
					// 5. Glow intensity
					sline >> token;
					if (token.substr(0,4) != BGD_FIELD_EOF)
						la_glow = atof(token.c_str());
					else
						la_glow = 0.f;
					la.itexture = la_texindex; la.icolor = la_colindex;
					la.glowing = la_glowing; la.glow_intensity = la_glow;
					gd.lvl_appearance.insert({ index, la });
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
			
	printf("Levels:\n");
	for (const auto& l : levels)
	{
		printf("\tUsing level %s\n", l.c_str());
	}

	printf("Textures:\n");
	for (const auto& p : textures)
	{
		printf("\tUsing texture %s at index %u\n", p.second.c_str(), p.first);
	};

	printf("Models:\n");
	for (const auto& p : models)
	{
		printf("\tUsing model %s at index %u\n", p.second.c_str(), p.first);
	};

	printf("Level appearance:\n");
	for (const auto& p : lvl_appearance)
	{
		const LevelAppearanceData& la = p.second;
		printf("\tIndex: %u TexIndex: %u ColorIndex: %u IsGlowing: %d GlowIntensity: %.2f\n",
			p.first, la.itexture, la.icolor, la.glowing, la.glow_intensity);
	};

	printf("Palettes:\n");
	for (const auto& p : palletes)
	{
		const glm::vec3& col = p.second;
		printf("\tColor R: %.2f %.2f %.2f at index %u\n", col.r, col.g, col.b, p.first);
	};
};