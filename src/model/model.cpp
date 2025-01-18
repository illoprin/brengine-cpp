#include "model.h"

using namespace b_Model;

void b_Model::ModelFromOBJ(
	std::string filename, std::string& name, std::vector<Triangle>& tris
)
{
	std::string filepath{MODEL_FILE_PATH(name)};


	FILE* obj_src = fopen(filepath.c_str(), "r");
	if (!obj_src)
	{
		perror(filepath.c_str());
		return;
	}

	std::vector<float> raw_vertices;
	std::vector<float> raw_texcoords;
	std::vector<float> raw_normals;

	char line[MAX_BUFFER];
	while (fgets(line, MAX_BUFFER, obj_src))
	{
		char* token = strtok(line, " ");
		if (!strcmp(token, "o"))
		{
			// Get name
			char* raw_name = strtok(NULL, " ");
			
			// Length of obj name without \n escape sequence
			size_t name_len = strlen(raw_name);
			
			char _name[name_len]; 
			strncpy(_name, raw_name, name_len);

			_name[name_len - 1] = '\0';

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
	// Close file
	fclose(obj_src);

	printf("OBJ Loader - loaded model with name %s from file %s\n", 
		name.c_str(), filepath.c_str());
};

void b_Model::printTriangles(ModelTriangles& tris)
{
	for (Triangle& tri : tris)
	{
		puts("- Face\n");
		for (unsigned i = 0; i < 3; i++)
		{
			printf("\tCoord: %.1f %.1f %.1f\n\tTexcoord: %.1f %.1f\n\tNormal: %.1f %.1f %.1f\n",
				tri.vertex[i].vx, tri.vertex[i].vy, tri.vertex[i].vz, 
				tri.vertex[i].tu, tri.vertex[i].tv, 
				tri.vertex[i].nx, tri.vertex[i].ny, tri.vertex[i].nz
			);
			putchar('\n');
		}
		putchar('\n');
	}
};