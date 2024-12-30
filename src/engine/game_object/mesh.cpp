#include "mesh.h"

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
	printf("OBJ Loader - loaded model with name %s from file %s", 
		name.c_str(), filepath.c_str());
};