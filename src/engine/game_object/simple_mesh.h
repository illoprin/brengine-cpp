#pragma once

#include "mesh.h"
#include "base_mesh.h"

class SimpleMesh : public BaseMesh
{
public:
	using BaseMesh::BaseMesh;

	// Destructor of ~BaseMesh copied automatically

	void initFromModel(ModelTriangles* tris);

	void draw(GLint mode) override;

	ModelTriangles* getTriangles() const override;
protected:
	ModelTriangles* triangles;
};