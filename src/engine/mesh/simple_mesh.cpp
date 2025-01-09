#include "simple_mesh.h"

#include "../engine.h"

void SimpleMesh::initFromModel(ModelTriangles* tris)
{
	if (tris->size() == 0)
		return;

	this->setTotal(tris->size() * 3);
	this->triangles = tris;

	size_t bytes = this->total_count * sizeof(ModelVertex);
	
	GLuint last_buffer = this->AddBuffer(tris->data(), bytes);

	this->SetDataPointer(
			last_buffer, 3, sizeof(ModelVertex), offsetof(ModelVertex, vx)
	);
	this->SetDataPointer(
			last_buffer, 2, sizeof(ModelVertex), offsetof(ModelVertex, tu)
	);
	this->SetDataPointer(
			last_buffer, 3, sizeof(ModelVertex), offsetof(ModelVertex, nx)
	);

	b_log->logf("[INFO] Mesh.%s - Simple initialization happend, total count of vertices is %u, buffer size is %lu bytes\n",
		this->name.c_str(), this->total_count, bytes);
};

ModelTriangles* SimpleMesh::getTriangles() const
{
	return this->triangles;
}