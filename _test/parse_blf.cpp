#include <cstdio>
#include <cmath>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <cstring>
#include <filesystem>

#define MAX_LINE 128

namespace fs = std::filesystem;

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

		void print()
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
		};
		size_t get_size()
		{
			return name.size() + verts.size() * sizeof(glm::vec2) + sectors.size() * sizeof(Sector) + walls.size() * sizeof(Wall);
		};
	};

	enum FileSeekType{
		NOTHING = 0,
		SEEK_VERTS,
		SEEK_WALLS,
		SEEK_SECTORS,
	};

	void FromBLF(std::string file, LevelData& ld)
	{
		std::string file_path{
			fs::path("assets") / "levels" / (file + ".blf")
		};
		FILE* ld_src = fopen(file_path.c_str(), "r");
		if (!ld_src)
		{
			fprintf(stderr, "b_Level::FromBLF - Could not open %s\n", file_path.c_str());
			return;
		}
		
		char line[MAX_LINE];
		char* token = NULL;
		unsigned current_line = 0u;
		FileSeekType type = FileSeekType::NOTHING;
		while (fgets(line, MAX_LINE, ld_src) != NULL)
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
						printf("Seeking Vertices on line %d\n", current_line);
						v_x = atof(token);
						token = strtok(NULL, " ");
						v_y = atof(token);
						ld.verts.push_back(glm::vec2(v_x, v_y));
						break;
					case SEEK_WALLS:
						printf("Seeking Walls on line %d\n", current_line);
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
						printf("Seeking Sectors on line %d\n", current_line);
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
						printf("Seeking Nothing on line %d\n", current_line);
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
};

namespace b_Model
{
	struct ModelVertex
	{
		float vx, vy, vz, tu, tv, nx, ny, nz;
	};
	struct Triangle
	{
		ModelVertex vertex[3];
	};
	typedef std::vector<Triangle> ModelTriangles;

	/*
		Indicate vertices of quads in CCW starting from left bottom
	*/
	void GetTrianglesFromFourVertices(
		glm::vec3 v1,
		glm::vec3 v2,
		glm::vec3 v3,
		glm::vec3 v4,
		glm::vec3 nq, // Normal
		Triangle& t1,
		Triangle& t2
	)
	{
		// Left top triangle
		t1 = {
			{
				{v1.x, v1.y, v1.z, 0, 0, nq.x, nq.y, nq.z},
				{v3.x, v3.y, v3.z, 1, 1, nq.x, nq.y, nq.z},
				{v4.x, v4.y, v4.z, 0, 1, nq.x, nq.y, nq.z},
			}
		};
		// Right bottom triangle
		t2 = {
			{
				{v3.x, v3.y, v3.z, 1, 1, nq.x, nq.y, nq.z},
				{v1.x, v1.y, v1.z, 0, 0, nq.x, nq.y, nq.z},
				{v2.x, v2.y, v2.z, 1, 0, nq.x, nq.y, nq.z},
			}
		};
	};

	
	void LevelToTriangles(
		b_Level::LevelData&    ld,
		std::vector<Triangle>& tris
	)
	{
		for (b_Level::Sector& sector : ld.sectors)
		{
			if (sector.wall_num == 0)
				continue;
			else
			{
				// Form Floor
				// TODO
				// Form Ceiling
				// TODO
				for (unsigned i = 0; i < sector.wall_num; i++)
				{
					// Indicate vertices of quads in counter clockwise order
					// starting from left bottom

					b_Level::Wall& w = ld.walls[sector.wall_start + i];
					glm::vec2& v1 = ld.verts[w.v1];
					glm::vec2& v2 = ld.verts[w.v2];
					// If it is not portal - form solid wall
					if (w.portal == 0)
					{
						// Define wall quad vertices
						glm::vec3 qv1{ v1.x, sector.floor_height, v1.y };
						glm::vec3 qv2{ v2.x, sector.floor_height, v2.y };
						glm::vec3 qv3{ v2.x, sector.ceiling_height, v2.y };
						glm::vec3 qv4{ v1.x, sector.ceiling_height, v1.y };
						// Get wall normal
						glm::vec2 wall_vec2d = v2 - v1;
						glm::vec3 normal = glm::cross(
							glm::normalize(glm::vec3(wall_vec2d.x, 0, wall_vec2d.y)),
							glm::vec3(0, 1, 0)
						);
						// Define wall triangles (left top, right bottom)

						// TODO: texcoords calculations

						// Left top triangle
						Triangle t_lt, t_rb;
						GetTrianglesFromFourVertices(qv1, qv2, qv3, qv4, normal, t_lt, t_rb);
						tris.push_back(t_lt);
						tris.push_back(t_rb);
					}
					// If it is portal - form top and bottom quad triangles
					else
					{
						b_Level::Sector& p_sector = ld.sectors[w.portal];

						// Get wall normal
						glm::vec2 wall_vec2d = v2 - v1;
						glm::vec3 normal = glm::cross(
							glm::normalize(glm::vec3(wall_vec2d.x, 0, wall_vec2d.y)),
							glm::vec3(0, 1, 0)
						);

						// Form bottom triangles, if floor height of current sector
						// is less then floor height of next sector
						glm::vec3 v1, v2, v3, v4;
						if (sector.floor_height < p_sector.floor_height)
						{
							v1 = { v1.x, sector.floor_height, v1.y };
							v2 = { v2.x, sector.floor_height, v2.y };
							v3 = { v2.x, p_sector.floor_height, v2.y };
							v4 = { v1.x, p_sector.floor_height, v1.y };

							// TODO: texcoords calculations

							Triangle tltb, trbb;
							GetTrianglesFromFourVertices(v1, v2, v3, v4, normal, tltb, trbb);
							tris.push_back(tltb);
							tris.push_back(trbb);
						}
						// Form top triangles, if ceiling height of current sector
						// is bigger then ceiling height of next sector  
						if (sector.ceiling_height > p_sector.ceiling_height)
						{
							v1 = { v1.x, p_sector.ceiling_height, v1.y };
							v2 = { v2.x, p_sector.ceiling_height, v2.y };
							v3 = { v2.x, sector.ceiling_height, v2.y };
							v4 = { v1.x, sector.ceiling_height, v1.y };
	
							// TODO: texcoords calculations

							Triangle tltt, trbt;
							GetTrianglesFromFourVertices(v1, v2, v3, v4, normal, tltt, trbt);
							tris.push_back(tltt);
							tris.push_back(trbt);
						}

					}
				}
			}

		}
		printf("Level %s total face count is: %d\n", ld.name.c_str(), tris.size());
	};
	void printTriangles(ModelTriangles& tris)
	{
		for (Triangle& tri : tris)
		{
			printf("- Face\n");
			for (unsigned i = 0; i < 3; i++)
			{
				printf(
					"\tCoord: %.1f %.1f %.1f\n\tTexcoord: %.1f %.1f\n\tNormal: %.1f %.1f %.1f\n",
					tri.vertex[i].vx, tri.vertex[i].vy, tri.vertex[i].vz, 
					tri.vertex[i].tu, tri.vertex[i].tv, 
					tri.vertex[i].nx, tri.vertex[i].ny, tri.vertex[i].nz
				);
				printf("\n");
			}
			printf("\n");
		}
	};
};

int main()
{
	// TARGET: BLF Data -> b_Model::Triangles
	b_Model::ModelTriangles d0_mesh;
	b_Level::LevelData d0;
	b_Level::FromBLF("d0", d0);
	d0.print();
	b_Model::LevelToTriangles(d0, d0_mesh);
	b_Model::printTriangles(d0_mesh);
	

	return 0;
};