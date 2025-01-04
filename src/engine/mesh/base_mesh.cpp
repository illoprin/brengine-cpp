#include "base_mesh.h"

BaseMesh::BaseMesh(Log* logger, const char* name)
{
	this->name = std::string(name);
	this->log = logger;
	this->total_count = 0u;

	glGenVertexArrays(1, &this->vao);
};

inline void BaseMesh::enable_attributes()
{
	for (GLuint& i : this->attrs_list)
		glEnableVertexAttribArray(i);
};

inline void BaseMesh::disable_attributes()
{
	for (GLuint& i : this->attrs_list)
		glDisableVertexAttribArray(i);
};

inline void BaseMesh::reset_attributes()
{
	this->attrs_list.clear();
};

GLuint BaseMesh::AddBuffer(void* data, size_t size_bytes)
{
	GLuint vbo;
	// Bind VAO object for setting up all attributes
	this->bind();

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size_bytes, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	// Unbind VAO object after setting up 
	glBindVertexArray(0);
	
	this->vbos.push_back(vbo);
	
	this->log->logf("[INFO] Mesh.%s - Generated new buffer object with index %u\n",
		this->name.c_str(), vbo);
	return vbo;
};

void BaseMesh::AddElementBuffer(std::vector<int>& elements)
{
	// TODO: Add element array object
};

void BaseMesh::SetDataPointer(
	GLuint    buffer,
	GLenum    type,
	unsigned  components,
	size_t    stride,
	size_t    offset
)
{

	// Bind VAO object for setting up all attributes
	this->bind();

	GLuint next_attr = this->attrs_list.size();

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glEnableVertexAttribArray(next_attr);
	glVertexAttribPointer(next_attr, components, type, GL_FALSE, stride, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO object after setting up 
	glBindVertexArray(0);

	this->log->logf("[INFO] Mesh.%s - Added attribute pointer with index %u\n",
		this->name.c_str(), next_attr);

	// New attribute writed, push its index to list
	this->attrs_list.push_back(next_attr);
};

void BaseMesh::SetDataPointerWithDivisior(
	GLuint   buffer_id,
	GLenum   type,
	unsigned components,
	size_t   stride,
	size_t   offset,
	GLuint   instances
)
{
	// Bind VAO object for setting up
	this->bind();

	GLuint next_attr = this->attrs_list.size();

	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	glEnableVertexAttribArray(next_attr);
	glVertexAttribPointer(next_attr, components, type, GL_FALSE, stride, (void*)offset);
	glVertexAttribDivisor(next_attr, instances);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO object after setting up
	glBindVertexArray(0);

	this->log->logf("[INFO] Mesh.%s - Added attribute pointer with divisor, index = %u, instance divisor = %u\n",
		this->name.c_str(), next_attr, instances);

	// New attribute writed, push its index to list
	this->attrs_list.push_back(next_attr);

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
		
		glDrawArrays(m, 0, this->total_count);
		
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
		
		glDrawArraysInstanced(m, 0, this->total_count, i);

		this->disable_attributes();

		glBindVertexArray(0);
	}
};


/* Methods to override */
ModelTriangles* BaseMesh::getTriangles() const
{
	return nullptr;
}
void BaseMesh::UpdateBuffer()
{
	// ...
};

unsigned BaseMesh::getTotal() const
{
	return this->total_count;
}

const std::vector<GLuint>& BaseMesh::BufferList() const
{
	return this->vbos;
};

GLuint BaseMesh::getLastBuffer() const
{
	return this->vbos[this->vbos.size() - 1];
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
	for (GLuint& vbo : this->vbos)
		glDeleteBuffers(1, &vbo);
	
	glDeleteVertexArrays(1, &this->vao);
	this->log->logf("[INFO] Mesh.%s - VAO & VBO cleared\n", this->name.c_str());
}

BaseMesh::~BaseMesh()
{
	this->clear();
}