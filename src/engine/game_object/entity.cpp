#include "entity.h"

using namespace b_GameObject;

Entity::Entity(std::string n)
{
	this->name = n;

	this->init_view();
};

Entity::Entity(std::string n, Transform t)
{
	this->name = n;
	this->transform = t;

	this->init_view();
};

void Entity::init_view()
{
	this->mesh = nullptr;
	this->program = nullptr;
	this->texture = nullptr;
	this->color = glm::vec4(1.f);
	printf("Entity.%s - Created!", this->name.c_str());
};

bool Entity::hasView()
{
	return (this->mesh != nullptr) && (this->program != nullptr);
};

void Entity::update()
{
	// ...
};

Entity::~Entity()
{

};

/* ==== Getters ==== */
BaseMesh* Entity::getMesh() const
{ return this->mesh; };
Program* Entity::getProgram() const
{ return this->program; };
glm::vec4 Entity::getColor() const
{ return this->color; }
TextureImage2D* Entity::getTexture() const
{ return this->texture; }
std::string Entity::getName() const
{ return (this->name); };
float Entity::getUVScaling() const
{
	return this->uv_scaling;
};

/* ==== Setters ==== */
void Entity::setTexture(TextureImage2D* t)
{
	this->texture = t;
};

void Entity::setColor(glm::vec3 c)
{
	this->color.r = c.r;
	this->color.g = c.g;
	this->color.b = c.b;
};
void Entity::setAlpha(float a)
{
	this->color.a = a;
};
void Entity::setUVScaling(float v)
{
	this->uv_scaling = v;
};
void Entity::setMesh(BaseMesh* m)
{
	this->mesh = m;
};
void Entity::setProgram(Program* p)
{
	this->program = p;
};