#pragma once

#include <util/cpp_includes.h>
#include <util/math_includes.h>

#define DIR_UP glm::vec3(0, 1, 0)
#define DIR_RIGHT glm::vec3(1, 0, 0)
#define DIR_FORWARD glm::vec3(0, 0, 1)

#define CAM_FOV 90.f
#define CAM_NEAR .01f
#define CAM_FAR 100.f

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