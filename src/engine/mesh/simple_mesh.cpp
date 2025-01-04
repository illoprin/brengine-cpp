#include "simple_mesh.h"

void SimpleMesh::initFromModel(ModelTriangles* tris)
{
	if (tris->size() == 0)
		return;

	this->setTotal(tris->size() * 3);
	this->triangles = tris;

	size_t bytes = this->total_count * sizeof(ModelVertex);
	
	GLuint last_buffer = this->AddBuffer(tris->data(), bytes);

	this->SetDataPointer(
			last_buffer, GL_FLOAT, 3, sizeof(ModelVertex), offsetof(ModelVertex, vx)
	);
	this->SetDataPointer(
			last_buffer, GL_FLOAT, 2, sizeof(ModelVertex), offsetof(ModelVertex, tu)
	);
	this->SetDataPointer(
			last_buffer, GL_FLOAT, 3, sizeof(ModelVertex), offsetof(ModelVertex, nx)
	);

	this->log->logf("[INFO] Mesh.%s - Simple initialization happend, total count of vertices is %u, buffer size is %lu bytes\n",
		this->name.c_str(), this->total_count, bytes);
};

void SimpleMesh::Draw(GLint mode)
{
	if (this->total_count > 2)
	{
		this->bind();

		this->enable_attributes();

		glDrawArrays(mode, 0, this->total_count);
		
		this->disable_attributes();

		glBindVertexArray(0);
	}
}

ModelTriangles* SimpleMesh::getTriangles() const
{
	return this->triangles;
}