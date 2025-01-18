#pragma once

#include <util/c_includes.h>
#include <util/cpp_includes.h>
#include <core/log.h>
#include <util/defs.h>
#include <util/gl_includes.h>
#include <model/model.h>

extern uint16_t b_draw_calls;
extern uint16_t b_total_vertices;

class BaseMesh
{
public:
	BaseMesh(const char* name);
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
	GLuint AddDynamicBuffer(
		void* 	data,
		size_t	size,
		size_t	reserve
	);

	/*
		Add vertex array attribute poiner
		AttributePoiner - is description of buffer data that you sent to shader on draw call
	*/
	void SetDataPointer(
		uch      index,
		unsigned components,
		size_t   stride,
		size_t   offset
	);
	
	/*
		Add vertex array attribute poiner
		AttributePoiner - is description of buffer data that you sent to shader on draw call
	*/
	void SetDataIntegerPointer(
		uch      index,
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
		uch       index,
		GLenum    type,
		unsigned  components,
		size_t    stride,
		size_t    offset,
		GLuint    instances = 1
	);

	// Delete VAO&VBO buffers, prepare to rewriting
	void clear();

	// Update (for dynamic VBO objects)
	void UpdateBuffer(
		uch     index,
		size_t  offset,
		size_t  size,
		void*   data
	);

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
	const std::vector<GLuint>& buffers() const;
	GLuint getLastBuffer() const;
	std::string getName() const;
protected:
	GLuint vao;
	std::string name;
	std::vector<GLuint> vbos;
	GLuint attributes = 0;
	unsigned total_count = 0;

	void enable_attributes();
	void disable_attributes();
	void reset_attributes();
	void bind();
	void unbind();
};

namespace b_Engine
{
	uint16_t getDrawCalls();
};
