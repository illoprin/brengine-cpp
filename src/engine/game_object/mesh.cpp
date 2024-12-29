#include "mesh.h"

BaseMesh::BaseMesh(Log* logger, const char* name)
{
	this->name = std::string(name);
	this->log = logger;
	this->total_count = 0u;
	this->attrs_list = 0u;

	glGenVertexArrays(1, &this->vao);
};

void BaseMesh::enable_attributes()
{
	for (GLuint i = 0u; i < this->attrs_list; i++)
		glEnableVertexAttribArray(i);
};

void BaseMesh::disable_attributes()
{
	for (GLuint i = 0u; i < this->attrs_list; ++i)
		glDisableVertexAttribArray(i);
};

void BaseMesh::simpleInit(std::vector<Triangle>* tris)
{
	if (tris->size() == 0)
		return;


	this->total_count = tris->size() * 3;
	this->tris = &(tris->data()[0]);

	size_t bytes = this->total_count * sizeof(ModelVertex);
	

	GLuint added_buffer = this->addb(&(this->tris->vertex[0].vx), bytes);
	this->addattr(
			added_buffer, 3, sizeof(ModelVertex), offsetof(ModelVertex, vx)
	);
	this->addattr(
			added_buffer, 2, sizeof(ModelVertex), offsetof(ModelVertex, tu)
	);
	this->addattr(
			added_buffer, 3, sizeof(ModelVertex), offsetof(ModelVertex, nx)
	);

	this->log->logf("[INFO] Mesh.%s - Simple initialization happend, total count of vertices is %u, buffer size is %lu bytes\n",
		this->name.c_str(), this->total_count, bytes);
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

void BaseMesh::bind()
{
	glBindVertexArray(this->vao);
}

void BaseMesh::draw()
{
	if (this->total_count > 0)
	{
		this->bind();
		
		this->enable_attributes();
		
		// printf("Mesh.%s - Draw call...\n", this->name.c_str());
		glDrawArrays(GL_TRIANGLES, 0, this->total_count);
		
		this->disable_attributes();

		glBindVertexArray(0);
	}
};

Triangle* BaseMesh::getMeshData() const
{
	if (this->tris != nullptr)
		return this->tris;
	else
	{
		this->log->logf("Mesh.%s.getMeshData: Mesh data not assigned\n");
		return NULL;
	}
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