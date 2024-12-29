#include <stdio.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define HELP_IO "\nCommands:\n\tlobj - load obj file and print data\n\twtb - write loaded triangles to binary\n\texit - exit from app\n\tipack - pack images from folder\n\tiwrite - gen image and write it to tmp folder\n\n"
#define DEVIDER_IO ">> "

struct ModelVertex
{
	float vx, vy, vz, tu, tv, nx, ny, nz;
};

struct Triangle
{
	ModelVertex vertices[3];
};

namespace ImageIO
{
	void GenBytes(std::vector<unsigned char>& bytes, unsigned width, unsigned height)
	{
		unsigned char r, g, b, a;
		for (unsigned i = 0; i < width * height; i++)
		{
			float factor = (float)i / (float)(width * height);
			
			// Generate gradient
			r = (unsigned char) floor(255.f * factor); 
			g = (unsigned char) floor(255.f * factor); 
			b = (unsigned char) floor(255.f * factor); 
			a = 255u;

			// Set values to vector
			// RGBA - stride is 4
			bytes[i * 4 + 0] = r; bytes[i * 4 + 1] = g;
			bytes[i * 4 + 2] = b; bytes[i * 4 + 3] = a;
		}
	};

	void WriteBytes(
		std::vector<unsigned char>& byteData,
		int width, 
		int height, 
		int components, 
		const char* path
	)
	{
		if (byteData.size() < components)
		{
			fprintf(stderr, "ImageIO - Could not write, byte data size is less then %d\n", components);
			return;
		}

		if (strlen(path) < 2)
		{
			fprintf(stderr, "ImageIO - Could not write, file path field is empty\n");
			return;
		}

		for (int i = 0; i < width * height; i++)
		{
			printf("Readed pixels: %u %u %u %u\n",
				byteData[i * 4 + 0], byteData[i * 4 + 1],
				byteData[i * 4 + 2], byteData[i * 4 + 3]);
		}
		
		if (!stbi_write_png(path, width, height, components, &byteData[0], components * width))
		{
			fprintf(stderr, "ImageIO - ERROR Could not write\n");
			return;
		}
		printf("ImageIO - png file with path %s writed!\n", path);
	};

	void GenAndWrite(unsigned width, unsigned height, std::string name)
	{
		std::string path{"tmp/" + name};
		std::vector<unsigned char> bytes(width * height * 4);
		ImageIO::GenBytes(bytes, width, height);
		ImageIO::WriteBytes(bytes, (int) width, (int) height, 4, path.c_str());
	}
};

namespace ModelIO
{
	void printTriangles(std::vector<Triangle>& tris)
	{
		for (Triangle& tri : tris)
		{
			printf("- Face\n");
			for (unsigned i = 0; i < 3; i++)
			{
				printf("\tCoord: %.1f %.1f %.1f\n\tTexcoord: %.1f %.1f\n\tNormal: %.1f %.1f %.1f\n",
					tri.vertices[i].vx, tri.vertices[i].vy, tri.vertices[i].vz, 
					tri.vertices[i].tu, tri.vertices[i].tv, 
					tri.vertices[i].nx, tri.vertices[i].ny, tri.vertices[i].nz
				);
				printf("\n");
			}
			printf("\n");
		}
	};

	void loadFromOBJ(std::string filename, std::string& name, std::vector<Triangle>& tris)
	{
		std::string filepath{"assets/" + filename + ".obj"};

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
					new_face.vertices[i].vx = raw_vertices[(vi - 1) * 3 + 0];
					new_face.vertices[i].vy = raw_vertices[(vi - 1) * 3 + 1];
					new_face.vertices[i].vz = raw_vertices[(vi - 1) * 3 + 2];

					// Add texcoords
					new_face.vertices[i].tu = raw_texcoords[(ti - 1) * 2 + 0];
					new_face.vertices[i].tv = raw_texcoords[(ti - 1) * 2 + 1];

					// Add normals
					new_face.vertices[i].nx = raw_normals[(ni - 1) * 3 + 0];
					new_face.vertices[i].ny = raw_normals[(ni - 1) * 3 + 1];
					new_face.vertices[i].nz = raw_normals[(ni - 1) * 3 + 2];
				}
				tris.push_back(new_face);
			}
		}
		printf("OBJ Loader - loaded model with name %s from file %s", 
			name.c_str(), filepath.c_str());
	};
};

int main()
{
	char command[128];
	std::vector<Triangle> obj_tris;
	

	bool started = true;
	while (started)
	{
		std::cout << HELP_IO << DEVIDER_IO;
		std::cin >> command;

		if (!strcmp(command, "exit"))
		{
			started = false;
		}
		else if (!strcmp(command, "lobj"))
		{
			std::string path;
			std::string name;

			// Read user input
			std::cin >> path;
			
			ModelIO::loadFromOBJ(path, name, obj_tris);

			std::cout << "Print results? (y/n)\n";
			std::cin >> command;
			if (!strcmp(command, "y"))
			{
				ModelIO::printTriangles(obj_tris);
			}		
		}
		else if (!strcmp(command, "iwrite"))
		{
			std::string filename;
			unsigned width, height;
			std::cin >> filename >> width >> height;
			ImageIO::GenAndWrite(width, height, filename);
		}

		obj_tris.clear();
	}
	return 0;
}