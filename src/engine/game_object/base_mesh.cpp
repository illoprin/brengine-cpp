#include "base_mesh.h"

BaseMesh::BaseMesh(Log* logger, const char* name)
{
	this->name = std::string(name);
	this->log = logger;
	this->total_count = 0u;
	this->attrs_list = 0u;

	glGenVertexArrays(1, &this->vao);
};

inline void BaseMesh::enable_attributes()
{
	for (GLuint i = 0u; i < this->attrs_list; i++)
		glEnableVertexAttribArray(i);
};

inline void BaseMesh::disable_attributes()
{
	for (GLuint i = 0u; i < this->attrs_list; ++i)
		glDisableVertexAttribArray(i);
};

inline void BaseMesh::reset_attributes()
{
	this->attrs_list = 0;
};

GLuint BaseMesh::addb(void* data, size_t size_bytes)
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

void BaseMesh::addi(std::vector<int>& elements)
{
	// TODO: Add element array object
};

void BaseMesh::addattr(GLuint buffer, unsigned components, size_t stride, size_t offset)
{

	// Bind VAO object for setting up all attributes
	this->bind();

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glEnableVertexAttribArray(this->attrs_list);
	glVertexAttribPointer(this->attrs_list, components, GL_FLOAT, GL_FALSE, stride, (void*)offset);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Unbind VAO object after setting up 
	glBindVertexArray(0);

	this->log->logf("[INFO] Mesh.%s - Added attribute pointer with index %u\n",
		this->name.c_str(), this->attrs_list);

	// New attribute writed, index increased by one
	++this->attrs_list;
};

inline void BaseMesh::bind()
{
	glBindVertexArray(this->vao);
}

inline void BaseMesh::draw(GLint mode)
{
	if (this->total_count > 0)
	{
		this->bind();
		
		this->enable_attributes();
		
		glDrawArrays(mode, 0, this->total_count);
		
		this->disable_attributes();

		glBindVertexArray(0);
	}
};

/* Methods to override */
ModelTriangles* BaseMesh::getTriangles() const
{
	return nullptr;
}
void BaseMesh::update()
{
	// ...
};

unsigned BaseMesh::getTotal() const
{
	return this->total_count;
}

GLuint BaseMesh::getLastBuffer() const
{
	return this->vbos.back();
}
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