#pragma once

#include "../utils/deps.h"
#include "../log.h"

#include "mesh.h"

class BaseMesh
{
public:
	BaseMesh(Log* logger, const char* name);
	~BaseMesh();

	// Delete copy operators
	BaseMesh(BaseMesh&) = delete;
	BaseMesh& operator=(BaseMesh&) = delete;
	BaseMesh& operator=(const BaseMesh&) = delete;

	// Create VAO and VBO entities
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

	// Update (for dynamic VBO objects)
	virtual void update();

	// Render
	virtual void draw(GLint mode = GL_TRIANGLES);

	// Set total count of verts
	void setTotal(unsigned verts);
	
	unsigned getTotal() const;
	GLuint getLastBuffer() const;
	std::string getName() const;
	virtual ModelTriangles* getTriangles() const;
protected:
	GLuint vao;
	std::string name;
	std::vector<GLuint> vbos;
	GLuint attrs_list;
	unsigned total_count;
	Log* log;

	void enable_attributes();
	void disable_attributes();
	void reset_attributes();
	void bind();
	void unbind();
};
