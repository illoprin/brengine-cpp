#include "mesh.h"

using namespace b_Model;

void b_Model::loadFromOBJ(
	std::string filename, std::string& name, std::vector<Triangle>& tris
)
{
	std::string filepath{
		fs::path(FS_ASSETS_PATH) / "models" / (filename + ".obj")
	};

	std::vector<float> raw_vertices;
	std::vector<float> raw_texcoords;
	std::vector<float> raw_normals;

	FILE* obj_src = fopen(filepath.c_str(), "r");
	if (!obj_src)
	{
		fprintf(stderr, "OBJ Loader - Could not open file with path %s\n", filepath.c_str());
		return;
	}

	char line[256];
	while (fgets(line, 256, obj_src) != NULL)
	{
		char* token = strtok(line, " ");
		if (strcmp(token, "o") == 0)
		{
			// Get name
			char* raw_name = strtok(NULL, " ");
			// Length of obj name without \n escape sequence
			unsigned name_len = strlen(raw_name) - 1;
			
			char _name[name_len]; 
			strncpy(_name, raw_name, name_len);

			name = {_name};
			printf("OBJ Loader - Loading obj named %s\n", _name);
		}
		if (strcmp(token, "v") == 0)
		{
			raw_vertices.push_back( (float)atof( strtok(NULL, " ") ) );
			raw_vertices.push_back( (float)atof( strtok(NULL, " ") ) );
			raw_vertices.push_back( (float)atof( strtok(NULL, " ") ) );
		}
		else if (strcmp(token, "vt") == 0)
		{
			raw_texcoords.push_back( (float)atof( strtok(NULL, " ") ) );
			raw_texcoords.push_back( (float)atof( strtok(NULL, " ") ) );
		}
		else if (strcmp(token, "vn") == 0)
		{
			raw_normals.push_back( (float)atof( strtok(NULL, " ") ) );
			raw_normals.push_back( (float)atof( strtok(NULL, " ") ) );
			raw_normals.push_back( (float)atof( strtok(NULL, " ") ) );
		}
		else if (strcmp(token, "f") == 0)
		{
			Triangle new_face;
			for (int i = 0; i < 3; i++)
			{
				char* face_info = strtok(NULL, " ");
				int vi, ti, ni;
				sscanf(face_info, "%d/%d/%d", &vi, &ti, &ni);
				// Add vertices
				new_face.vertex[i].vx = raw_vertices[(vi - 1) * 3 + 0];
				new_face.vertex[i].vy = raw_vertices[(vi - 1) * 3 + 1];
				new_face.vertex[i].vz = raw_vertices[(vi - 1) * 3 + 2];

				// Add texcoords
				new_face.vertex[i].tu = raw_texcoords[(ti - 1) * 2 + 0];
				new_face.vertex[i].tv = raw_texcoords[(ti - 1) * 2 + 1];

				// Add normals
				new_face.vertex[i].nx = raw_normals[(ni - 1) * 3 + 0];
				new_face.vertex[i].ny = raw_normals[(ni - 1) * 3 + 1];
				new_face.vertex[i].nz = raw_normals[(ni - 1) * 3 + 2];
			}
			tris.push_back(new_face);
		}
	}
	// Close obj file source
	fclose(obj_src);

	printf("OBJ Loader - loaded model with name %s from file %s\n", 
		name.c_str(), filepath.c_str());
};


/*
	Indicate vertices of quads in CCW starting from left bottom
*/
static void TriangulateWall(
	glm::vec3 v1,
	glm::vec3 v2,
	glm::vec3 v3,
	glm::vec3 v4,
	glm::vec3 nq,
	Triangle& t1,
	Triangle& t2,
	float wl, // Wall length
	float wh // Wall height
)
{
	v1.x *= LVL_SCALING; v1.z *= LVL_SCALING;
	v2.x *= LVL_SCALING; v2.z *= LVL_SCALING;
	v3.x *= LVL_SCALING; v3.z *= LVL_SCALING;
	v4.x *= LVL_SCALING; v4.z *= LVL_SCALING;

	float _wl = wl * LVL_SCALING;
	float _wh = wh * LVL_SCALING;
	
	// Left top triangle
	t1 = {
		{
			{v1.x, v1.y, v1.z, 0, 0, nq.x, nq.y, nq.z},
			{v4.x, v4.y, v4.z, 0, _wh, nq.x, nq.y, nq.z},
			{v3.x, v3.y, v3.z, _wl, _wh, nq.x, nq.y, nq.z},
		}
	};
	// Right bottom triangle
	t2 = {
		{
			{v1.x, v1.y, v1.z, 0, 0, nq.x, nq.y, nq.z},
			{v3.x, v3.y, v3.z, _wl, _wh, nq.x, nq.y, nq.z},
			{v2.x, v2.y, v2.z, _wl, 0, nq.x, nq.y, nq.z},
		}
	};
};


void b_Model::LevelToTriangles (
	b_Level::LevelData& ld,
	ModelTriangles&     tris
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

				// Get wall normal
				glm::vec2 wall_vec2d = v2 - v1;
				glm::vec3 normal = glm::cross(
					glm::vec3(0, 1, 0),
					glm::normalize(glm::vec3(wall_vec2d.x, 0, wall_vec2d.y))
				);
				// Wall height if not portal - abs differecne of ceiling height and floor height
				float h_delta;

				// If it is not portal - form solid wall
				if (w.portal == 0)
				{
					
					// Define wall quad vertices
					glm::vec3 qv1{ v1.x, sector.floor_height, v1.y };
					glm::vec3 qv2{ v2.x, sector.floor_height, v2.y };
					glm::vec3 qv3{ v2.x, sector.ceiling_height, v2.y };
					glm::vec3 qv4{ v1.x, sector.ceiling_height, v1.y };
					
					h_delta = fabs(sector.ceiling_height - sector.floor_height);

					// Define wall triangles (left top, right bottom)
					Triangle t_lt, t_rb;
					TriangulateWall(
						qv1, qv2, qv3, qv4, normal,
						t_lt, t_rb,
						glm::length(wall_vec2d), h_delta 
					);
					tris.push_back(t_lt);
					tris.push_back(t_rb);
				}
				// If it is portal - form top and bottom quad triangles
				else
				{
					b_Level::Sector& p_sector = ld.sectors[w.portal];

					// Form bottom triangles, if floor height of current sector
					// is less then floor height of next sector
					if (sector.floor_height < p_sector.floor_height)
					{
						glm::vec3 vqb1 = { v1.x, sector.floor_height, v1.y };
						glm::vec3 vqb2 = { v2.x, sector.floor_height, v2.y };
						glm::vec3 vqb3 = { v2.x, p_sector.floor_height, v2.y };
						glm::vec3 vqb4 = { v1.x, p_sector.floor_height, v1.y };

						h_delta = p_sector.floor_height - sector.floor_height;

						Triangle tltb, trbb;
						TriangulateWall(
							vqb1, vqb2, vqb3, vqb4, normal,
							tltb, trbb,
							glm::length(wall_vec2d), h_delta
						);
						tris.push_back(tltb);
						tris.push_back(trbb);
					}
					// Form top triangles, if ceiling height of current sector
					// is bigger then ceiling height of next sector  
					if (sector.ceiling_height > p_sector.ceiling_height)
					{
						glm::vec3 vqt1 = { v1.x, p_sector.ceiling_height, v1.y };
						glm::vec3 vqt2 = { v2.x, p_sector.ceiling_height, v2.y };
						glm::vec3 vqt3 = { v2.x, sector.ceiling_height, v2.y };
						glm::vec3 vqt4 = { v1.x, sector.ceiling_height, v1.y };

						h_delta = sector.ceiling_height - p_sector.ceiling_height;

						Triangle tltt, trbt;
						TriangulateWall(
							vqt1, vqt2, vqt3, vqt4, normal,
							tltt, trbt,
							glm::length(wall_vec2d), h_delta
						);
						tris.push_back(tltt);
						tris.push_back(trbt);
					}

				}
			}
		}

	}
	printf("Level %s total face count is: %ld\n", ld.name.c_str(), tris.size());
};

void b_Model::printTriangles(ModelTriangles& tris)
{
	for (Triangle& tri : tris)
	{
		printf("- Face\n");
		for (unsigned i = 0; i < 3; i++)
		{
			printf("\tCoord: %.1f %.1f %.1f\n\tTexcoord: %.1f %.1f\n\tNormal: %.1f %.1f %.1f\n",
				tri.vertex[i].vx, tri.vertex[i].vy, tri.vertex[i].vz, 
				tri.vertex[i].tu, tri.vertex[i].tv, 
				tri.vertex[i].nx, tri.vertex[i].ny, tri.vertex[i].nz
			);
			printf("\n");
		}
		printf("\n");
	}
};