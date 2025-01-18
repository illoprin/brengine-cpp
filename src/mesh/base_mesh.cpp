#include "base_mesh.h"

#include <core/context.h>

uint16_t b_draw_calls = 0;
uint16_t b_total_vertices = 0;

BaseMesh::BaseMesh(const char* name)
{
	this->name = std::string(name);
	total_count = 0u;

	glGenVertexArrays(1, &this->vao);
};

inline void BaseMesh::enable_attributes()
{
	for (unsigned i = 0; i < attributes; ++i)
		glEnableVertexAttribArray(i);
};

inline void BaseMesh::disable_attributes()
{
	for (unsigned i = 0; i < attributes; ++i)
		glDisableVertexAttribArray(i);
};

inline void BaseMesh::reset_attributes()
{
	attributes = 0u;
};

GLuint BaseMesh::AddBuffer(void* data, size_t size_bytes)
{
	GLuint vbo;

	// Bind VAO object for setting up its buffer
	bind();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Unbind VAO object after setting up its buffer 
	glBindVertexArray(0);
	
	vbos.push_back(vbo);
	
	if (b_CheckError() == GL_NO_ERROR);
		LOG_MSG("Mesh.%s generated new buffer index = %u size = %lu",
			name.c_str(), vbo, size_bytes);
	
	return vbo;
};

GLuint BaseMesh::AddDynamicBuffer(
	void* 	data,
	size_t	size,
	size_t	reserve
)
{
	GLuint vbo;

	// Bind VAO object for setting up its buffer
	this->bind();
	
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, (size + reserve), data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	this->vbos.push_back(vbo);
	
	if (b_CheckError() == GL_NO_ERROR);
		LOG_MSG("Mesh.%s generated new dynamic buffer index = %u reserve %lu bytes",
			name.c_str(), vbo, (size + reserve));

	// Unbind VAO object after setting up its buffer 
	glBindVertexArray(0);
	return vbo;
};

void BaseMesh::SetDataPointer(
	uch       index,
	unsigned  components,
	size_t    stride,
	size_t    offset
)
{

	// Bind VAO object for setting up all attributes
	this->bind();

	glBindBuffer(GL_ARRAY_BUFFER, vbos[index]);
	glEnableVertexAttribArray(attributes);
	glVertexAttribPointer(attributes, components, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO object after setting up 
	glBindVertexArray(0);

	if (b_CheckError() == GL_NO_ERROR)
		LOG_MSG("Mesh.%s added attribute pointer with index %u", name.c_str(), attributes);

	// New attribute writed
	attributes++;
};

void BaseMesh::SetDataIntegerPointer(
	uch       index,
	unsigned  components,
	size_t    stride,
	size_t    offset
)
{

	// Bind VAO object for setting up all attributes
	this->bind();

	glBindBuffer(GL_ARRAY_BUFFER, vbos[index]);
	glEnableVertexAttribArray(attributes);
	glVertexAttribIPointer(attributes, components, GL_INT, stride, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO object after setting up 
	glBindVertexArray(0);

	if (b_CheckError() == GL_NO_ERROR)
		LOG_MSG("Mesh.%s added attribute integer pointer with index %u", name.c_str(), attributes);

	// New attribute writed
	attributes++;
};

void BaseMesh::SetDataPointerWithDivisior(
	uch      index,
	GLenum   type,
	unsigned components,
	size_t   stride,
	size_t   offset,
	GLuint   instances
)
{
	// Bind VAO object for setting up
	this->bind();

	glBindBuffer(GL_ARRAY_BUFFER, vbos[index]);
	glEnableVertexAttribArray(attributes);
	glVertexAttribPointer(attributes, components, type, GL_FALSE, stride, (void*)offset);
	glVertexAttribDivisor(attributes, instances);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO object after setting up
	glBindVertexArray(0);

	if (b_CheckError() == GL_NO_ERROR)
		LOG_MSG("Mesh.%s - added attribute pointer with divisor, index = %u, divisor = %u",
			name.c_str(), attributes, instances);
	
	// New attribute writed
	attributes++;
};

inline void BaseMesh::bind()
{
	glBindVertexArray(this->vao);
}

inline void BaseMesh::Draw(GLint m)
{
	if (this->total_count > 0)
	{
		this->bind();
		
		this->enable_attributes();
		
		glDrawArrays(m, 0, total_count);
		++b_draw_calls;
		b_total_vertices += total_count;

		this->disable_attributes();

		glBindVertexArray(0);
	}
};


inline void BaseMesh::DrawInstanced(GLuint i, GLint m)
{
	if (this->total_count > 0)
	{
		this->bind();
		
		this->enable_attributes();
		
		glDrawArraysInstanced(m, 0, total_count, i);
		++b_draw_calls;
		b_total_vertices += total_count * i;

		this->disable_attributes();

		glBindVertexArray(0);
	}
};

void BaseMesh::UpdateBuffer(
	uch     index,
	size_t  offset,
	size_t  size,
	void*   data
)
{
	this->bind();

	glBindBuffer(GL_ARRAY_BUFFER, vbos[index]);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
};

unsigned BaseMesh::getTotal() const
{
	return this->total_count;
}

const std::vector<GLuint>& BaseMesh::buffers() const
{
	return vbos;
};

GLuint BaseMesh::getLastBuffer() const
{
	return vbos[vbos.size() - 1];
};

std::string BaseMesh::getName() const
{
	return this->name;
}

void BaseMesh::setTotal(unsigned verts)
{
	this->total_count = verts;
}

void BaseMesh::clear()
{
	glDeleteBuffers(vbos.size(), vbos.data());
	LOG_MSG("Mesh.%s cleared buffers %lu", name.c_str(), vbos.size());
	glDeleteVertexArrays(1, &vao);
	LOG_MSG("Mesh.%s released", name.c_str());
}

BaseMesh::~BaseMesh()
{
	clear();
}