#pragma once

#include "../utils/deps.h"
#include "base_mesh.h"
#include "../program.h"
#include "../texture.h"


class Entity
{

public:
	Entity(BaseMesh* mesh, Program* prog);
	Entity(BaseMesh* m, Program* p, glm::vec3 pos, glm::vec3 scl, glm::vec3 rot);
	~Entity();

	void setTexture(TextureImage2D* texture);
	void setColor(glm::vec3 color);
	void setAlpha(float value);
	
	TextureImage2D* getTexture() const;
	glm::vec3 getColor() const;
	float getAlpha() const;

	void setPosition(glm::vec3 pos);
	void setScale(glm::vec3 scl);
	void setRotation(glm::vec3 rot);

	void move(glm::vec3 pos);
	void rotate(glm::vec3 rot);
	void resize(glm::vec3 scl);

	void update();

	BaseMesh* getMesh() const;
	Program* getProgram() const;
	glm::mat4 getModelMatrix() const;

protected:
	BaseMesh* mesh;
	Program* program;

	// Rendering
	TextureImage2D* texture;
	glm::vec3 color;
	float alpha;

	// Transformation info
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	glm::mat4 model;

	void update_model();
	void init_view();
};