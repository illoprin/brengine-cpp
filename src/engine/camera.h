#pragma once

#include "utils/deps.h"

class Camera
{
public:
	Camera() {};
	Camera(glm::vec3 pos);
	~Camera();

	void update();

	void setPosition(glm::vec3 pos);
	void move(glm::vec3 delta);
	void rotate(float p, float y);

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	float getPitch();
	float getYaw();
	
	glm::vec3 getPosition();
	glm::vec3 getForward();
	glm::vec3 getRight();
	glm::vec3 getUp();
private:
	void update_vectors();
	void update_matrices();

	float pitch = 0.f;
	float yaw = 90.f;
	glm::vec3 position{0.f};

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 forward;
	glm::vec3 right;
	glm::vec3 up;
};