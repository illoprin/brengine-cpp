#pragma once

#include "../deps.h"
#include "../log.h"

struct ModelVertex {
	float vx, vy, vz, tu, tv, nx, ny, nz;
};

struct Triangle {
	struct ModelVertex vertex[3];
};

class BaseMesh
{
public:
	BaseMesh(Log* logger, const char* name);
	~BaseMesh();

	// Delete copy operators
	BaseMesh(BaseMesh&) = delete;
	BaseMesh& operator=(BaseMesh&) = delete;
	BaseMesh& operator=(const BaseMesh&) = delete;

	void simpleInit(std::vector<Triangle>* tris);
	GLuint addb(void* data, size_t size_bytes);
	void addi(std::vector<int>& elements);
	void addattr(
		GLuint buffer,
		unsigned components,
		size_t stride,
		size_t offset
	);
	// Delete VAO&VBO buffers, prepare to rewriting
	void clear();
	void draw();

	unsigned getTotal();
	Triangle* getMeshData();
private:
	GLuint vao;
	std::string name;
	std::vector<GLuint> vbos;
	GLuint attrs_list;
	Triangle* tris; // Pointer to first element in memory
	unsigned total_count;
	Log* log;

	void enable_attributes();
	void disable_attributes();
	void bind();
	void unbind();
};
