#pragma once

#include "model.h"
#include "base_mesh.h"

class SimpleMesh : public BaseMesh
{
public:
	using BaseMesh::BaseMesh;

	// Destructor of BaseMesh copied automatically

	void initFromModel(ModelTriangles* tris);

	void Draw(GLint mode) override;

	ModelTriangles* getTriangles() const override;
protected:
	ModelTriangles* triangles;
};