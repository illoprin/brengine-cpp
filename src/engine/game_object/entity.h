#pragma once

#include "../deps.h"
#include "mesh.h"
#include "../program.h"


class Entity
{

public:
	Entity(BaseMesh* mesh, Program* prog)
		: mesh(mesh), program(prog) {};
	~Entity();

	void setPosition(glm::vec3 pos);
	void setScale(glm::vec3 scl);
	void setRotation(glm::vec3 rot);

	void move(glm::vec3 pos);
	void rotate(glm::vec3 rot);
	void resize(glm::vec3 scl);

	void update();

	BaseMesh* getMesh() const;
	Program* getProgram() const;
private:
	BaseMesh* mesh;
	Program* program;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model;

	void update_model();
};