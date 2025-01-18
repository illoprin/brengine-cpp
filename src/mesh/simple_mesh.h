#pragma once

#include <model/model.h>
#include <mesh/base_mesh.h>
#include <core/log.h>

class SimpleMesh : public BaseMesh
{
public:
	using BaseMesh::BaseMesh;

	// Destructor of BaseMesh copied automatically

	void initFromModel(b_Model::ModelTriangles* tris);

	b_Model::ModelTriangles* getTriangles() const;
protected:
	b_Model::ModelTriangles* triangles;
};