#include "camera.h"

#include <core/engine.h>

Camera::Camera(glm::vec3 pos)
{
	this->setPosition(pos);
};

void Camera::update_vectors()
{
	// X
	this->forward.x = cosf(glm::radians(this->yaw)) * cosf(glm::radians(this->pitch));
	// Y
	this->forward.y = sinf(glm::radians(this->pitch));
	// Z
	this->forward.z = cosf(glm::radians(this->pitch)) * sinf(glm::radians(this->yaw));

	this->forward = glm::normalize(this->forward);

	// Compute camera right
	this->right = glm::normalize(glm::cross(DIR_UP, this->forward));

	// Compute camera up
	this->up = glm::normalize(glm::cross(this->forward, this->right));
};

void Camera::update_matrices()
{
	float aspect = b_Engine::getAspect();
	this->projection = glm::perspective(
		glm::radians(CAM_FOV),
		aspect,
		CAM_NEAR, CAM_FAR
	);
	this->view = glm::lookAt(
		this->position, (this->position + this->forward), this->up
	);
};

void Camera::update()
{	
	this->update_vectors();
	this->update_matrices();	
};

void Camera::setPosition(glm::vec3 pos)
{
	this->position = pos;
};
void Camera::move(glm::vec3 delta)
{
	this->position += delta;
};
void Camera::rotate(float p, float y)
{
	this->pitch =
		glm::clamp(this->pitch + p, -89.9f, 89.9f);
	this->yaw += y;
};

glm::mat4 Camera::getViewMatrix()
{
	return this->view;
};
glm::mat4 Camera::getProjectionMatrix()
{
	return this->projection;
};

float Camera::getPitch()
{
	return this->pitch;
};
float Camera::getYaw()
{
	return this->yaw;
};

glm::vec3 Camera::getPosition()
{
	return this->position;
};
glm::vec3 Camera::getForward()
{
	return this->forward;
};
glm::vec3 Camera::getRight()
{
	return this->right;
};
glm::vec3 Camera::getUp()
{
	return this->up;
};

Camera::~Camera()
{

};