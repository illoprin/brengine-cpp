#include "entity.h"

Entity::Entity(BaseMesh* mesh, Program* prog)
{
	this->mesh = mesh;
	this->program = prog;

	this->position = glm::vec3(0.f, 0.f, 0.f);
	this->rotation = glm::vec3(0.f, 0.f, 0.f);
	this->scale = glm::vec3(1.f);

	this->init_view();
};

Entity::Entity(
	BaseMesh* m, 
	Program* p,
	glm::vec3 pos, 
	glm::vec3 rot,
	glm::vec3 scl
)
{
	this->mesh = m;
	this->program = p;

	this->position = pos;
	this->rotation = rot;
	this->scale = scl;

	this->init_view();
};

void Entity::setTexture(TextureImage2D* tex) { this->texture = tex; }
void Entity::setColor(glm::vec3 col) { this->color = col; }
void Entity::setAlpha(float value) { this->alpha = value; }

float Entity::getAlpha() const { return this->alpha; }
glm::vec3 Entity::getColor() const { return this->color; }
TextureImage2D* Entity::getTexture() const { return this->texture; }

Entity::~Entity()
{

};

void Entity::update()
{
	this->update_model();
};

void Entity::update_model()
{
	model = glm::mat4(1.f);

	// Applying transformations order
	// 		position -> rotation -> scale
	model = glm::translate(model, this->position);
	
	model = glm::rotate(model, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
	model = glm::rotate(model, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
	model = glm::rotate(model, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
	
	model = glm::scale(model, this->scale);
};

void Entity::init_view()
{
	this->texture = nullptr;
	this->color = glm::vec3(1.f);
	this->alpha = 1.f;
}

void Entity::setPosition(glm::vec3 pos)
{
	this->position = pos;
};

void Entity::setRotation(glm::vec3 rot)
{
	this->rotation = rot;
};

void Entity::setScale(glm::vec3 scl)
{
	this->scale = scl;
};

void Entity::move(glm::vec3 delta)
{
	this->position += delta;
};

void Entity::rotate(glm::vec3 delta)
{
	this->rotation += delta;
};

void Entity::resize(glm::vec3 delta)
{
	this->scale += delta;
};

glm::mat4 Entity::getModelMatrix() const
{
	return this->model;
};

BaseMesh* Entity::getMesh() const
{
	return this->mesh;
};

Program* Entity::getProgram() const
{
	return this->program;
};