#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <string>
#include <fstream>
#include <istream>
#include <vector>
#include <sstream>
#include <filesystem>
#include <map>

#include <glm/glm.hpp>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace fs = std::filesystem;

#define BGD_FIELD_EOF "NULL"
#define BGD_PALETTE_TILE_SIZE (10)

namespace b_Utils
{
	void file_name_and_ext_from_str(
		std::string src, std::string& filename, std::string& ext
	)
	{
		filename = {src.substr(0, src.find('.'))};
		ext = {src.substr(src.find('.') + 1)};
	}
};

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
		std::string game_name = "nothing";
		std::vector<glm::vec2> verts;
		std::map<uint16_t, Sector> sectors{};
		std::map<uint16_t, Wall> walls;

		void print()
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
		};
		size_t get_size()
		{
			return name.size() + game_name.size()
				+ verts.size() * sizeof(glm::vec2)
				+ sectors.size() * sizeof(Sector) + sectors.size() * sizeof(uint16_t)
				+ walls.size() * sizeof(Wall) + walls.size() * sizeof(uint16_t);
		};
		const bool empty()
		{
			return (verts.size()   > 1) ||
			       (sectors.size() > 1) ||
				   (walls.size()   > 1);
		}
		void clear()
		{
			verts.clear();
			sectors.clear();
			walls.clear();
		};
	};

	enum BLFSeekType{
		BLF_NOTHING = 0,
		BLF_VERTS,
		BLF_WALLS,
		BLF_SECTORS,
	};

	void FromBLF(std::string file, LevelData& ld)
	{
		std::string file_path{
			fs::path("assets") / "levels" / (file + ".blf")
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
};

namespace b_Game
{
	struct LevelAppearanceData
	{
		uint16_t itexture; // Index of texture
		uint16_t icolor; // Index of color
		bool glowing;
		float glow_intensity;
	};

	struct GameData
	{
		std::string game_name;
		std::string file_name;
		std::vector<std::string> levels;
		std::map<uint16_t, std::string> textures;
		std::map<uint16_t, std::string> models;
		std::map<uint16_t, glm::vec3> palletes;
		std::map<uint16_t, LevelAppearanceData> lvl_appearance;

		void print()
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
	};

	enum BGDSeekType
	{
		BGD_NOTHING = 0,
		BGD_ENT_APPEARANCE,
		BGD_LVL_APPEARANCE,
		BGD_PALLETE
	};

	void GameDataFromBGD(const std::string file_name, GameData& gd)
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

	class GameDataHandler
	{
	public:
		GameDataHandler(const std::string bgdname)
		{
			GameDataFromBGD(bgdname, gd);
			buildPalette();
			gd.print();
		};
		~GameDataHandler()
		{
			ld.clear();
		};
		void loadLevel(uint16_t index)
		{
			if (!ld.empty())
				ld.clear();
			b_Level::FromBLF(gd.levels[index], ld);
			if (ld.game_name != gd.file_name)
			{
				printf(
					"b_Game::GameDataHandler - Warning - Could not resolve level %s. It is from another game named %s\n",
					ld.name.c_str(), ld.game_name.c_str());
				ld.clear();
			}
			else
				ld.print();
		};
		inline const GameData& getGameData()
		{
			return gd;
		};
		inline const b_Level::LevelData& getCurrentLevel()
		{
			return ld;
		};
	private:
		void buildPalette()
		{
			// Palette image width, height, xOffset
			uint16_t pheight = BGD_PALETTE_TILE_SIZE, poffset = BGD_PALETTE_TILE_SIZE;
			uint16_t pwidth = gd.palletes.size() * poffset;

			unsigned char* pixels = (unsigned char*)malloc(pheight * pwidth * 3);
			unsigned curx = 0;
			for (const auto& p : gd.palletes)
			{
				const glm::vec3& colc = p.second;
				unsigned char r = (unsigned char)(colc.r * 255.f);
				unsigned char g = (unsigned char)(colc.g * 255.f);
				unsigned char b = (unsigned char)(colc.b * 255.f);
				unsigned byte_offset = 0u;
				for (unsigned i = 0; i < pheight; i++)
					for (unsigned j = curx; j < curx + poffset; j++)
					{
						byte_offset = j + i * pwidth;
						pixels[byte_offset * 3 + 0] = r;	
						pixels[byte_offset * 3 + 1] = g;	
						pixels[byte_offset * 3 + 2] = b;	
					}
				curx += poffset;
			}
			if (stbi_write_bmp("tmp/palette.bmp", pwidth, pheight, 3, pixels))
			{
				printf("b_Game::GameDataHandler - Palette writed, width is %u height is %u\n",
					pwidth, pheight);
			}
			free(pixels);
		};
		GameData gd;
		b_Level::LevelData ld;
	};
}

int main()
{
	b_Game::GameDataHandler gdh{"initial"};
	gdh.loadLevel(0);
	return 0;
}