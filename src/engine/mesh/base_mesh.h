#pragma once

#include "../utils/deps.h"
#include "../log.h"

#include "model.h"

using namespace b_Model;

class BaseMesh
{
public:
	BaseMesh(Log* logger, const char* name);
	virtual ~BaseMesh();

	// Delete copy operators
	BaseMesh(BaseMesh&) = delete;
	BaseMesh& operator=(BaseMesh&) = delete;
	BaseMesh& operator=(const BaseMesh&) = delete;

	// Create VBO entity
	GLuint AddBuffer(
		void*   data,
		size_t  size_bytes
	);
	void AddElementBuffer(std::vector<int>& elements);

	/*
		Add vertex array attribute poiner
		AttributePoiner - is description of buffer data that you sent to shader on draw call
	*/
	void SetDataPointer(
		GLuint   buffer_id,
		GLenum   type,
		unsigned components,
		size_t   stride,
		size_t   offset
	);

	/*
		Add vertex array with divisior.
		That means how much times we send data to vertex shader
		You can use it for instanced rendering:
			Add buffer with model matrices ->
			Create attrib poiner to models ->
			Add divisor for this attribute
	*/
	void SetDataPointerWithDivisior(
		GLuint    buffer_id,
		GLenum    type,
		unsigned  components,
		size_t    stride,
		size_t    offset,
		GLuint    instances = 1
	);

	// Delete VAO&VBO buffers, prepare to rewriting
	void clear();

	// Update (for dynamic VBO objects)
	virtual void UpdateBuffer();

	// Render single mesh
	virtual void Draw(
		GLint  mode = GL_TRIANGLES
	);

	// Render instanced
	virtual void DrawInstanced(
		GLuint instances,
		GLint  mode = GL_TRIANGLES
	);


	// Set total count of verts
	void setTotal(unsigned verts);
	
	unsigned getTotal() const;
	const std::vector<GLuint>& BufferList() const;
	GLuint getLastBuffer() const;
	std::string getName() const;
	virtual ModelTriangles* getTriangles() const;
protected:
	GLuint vao;
	std::string name;
	std::vector<GLuint> vbos;
	std::vector<GLuint> attrs_list;
	unsigned total_count;
	Log* log;

	void enable_attributes();
	void disable_attributes();
	void reset_attributes();
	void bind();
	void unbind();
};
