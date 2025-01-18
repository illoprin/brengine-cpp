#include "simple_mesh.h"

using namespace b_Model;

void SimpleMesh::initFromModel(ModelTriangles* tris)
{
	if (tris->size() == 0)
		return;


	setTotal(tris->size() * 3);
	this->triangles = tris;

	size_t bytes = total_count * sizeof(ModelVertex);
	
	AddBuffer(tris->data(), bytes);
	
	SetDataPointer(
			0, 3, sizeof(ModelVertex), offsetof(ModelVertex, vx)
	);
	SetDataPointer(
			0, 2, sizeof(ModelVertex), offsetof(ModelVertex, tu)
	);
	SetDataPointer(
			0, 3, sizeof(ModelVertex), offsetof(ModelVertex, nx)
	);

	LOG_MSG("Mesh.%s total count of vertices = %u, buffer size = %lu bytes",
		name.c_str(), total_count, bytes);
};

ModelTriangles* SimpleMesh::getTriangles() const
{
	return this->triangles;
}