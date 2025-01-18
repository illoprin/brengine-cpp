#include "editor_controller.h"

#include <core/engine.h>

EditorController::EditorController()
{
	this->camera = new Camera;
};

EditorController::~EditorController()
{
	delete this->camera;
};

void EditorController::update()
{
	this->update_states();
	if (b_Engine::isGameMode())
	{
		this->handle_key_events();
		this->handle_mouse_events();
	};
	this->camera->update();
};

void EditorController::update_states()
{
	GLFWwindow* wnd = b_Engine::getWindow();
	this->states[PL_M_FORWARD] = glfwGetKey(wnd, GLFW_KEY_W);
	this->states[PL_M_BACKWARD] = glfwGetKey(wnd, GLFW_KEY_S);
	this->states[PL_M_LEFT] = glfwGetKey(wnd, GLFW_KEY_A);
	this->states[PL_M_RIGHT] = glfwGetKey(wnd, GLFW_KEY_D);
	this->states[PL_M_MODIFY] = glfwGetKey(wnd, GLFW_KEY_LEFT_SHIFT);
};
void EditorController::handle_key_events()
{
	float vel = b_Engine::getClock()->getDeltaTime() * PLAYER_SPEED;

	if (this->states[PL_M_MODIFY] == GLFW_PRESS)
		vel *= PLAYER_SPEED_MODIFER;
	if (this->states[PL_M_FORWARD] == GLFW_PRESS)
		this->camera->move(this->camera->getForward() * vel);
	if (this->states[PL_M_BACKWARD] == GLFW_PRESS)
		this->camera->move(-this->camera->getForward() * vel);
	if (this->states[PL_M_LEFT] == GLFW_PRESS)
		this->camera->move(this->camera->getRight() * vel);
	if (this->states[PL_M_RIGHT] == GLFW_PRESS)
		this->camera->move(-this->camera->getRight() * vel);
};
void EditorController::handle_mouse_events()
{
	Input* io = b_Engine::getIO();
	float factor = 0.05f;
	this->camera->rotate(
		-(float)io->getMouse().m_delta_y * factor,
		(float)io->getMouse().m_delta_x * factor
	);
};

Camera* EditorController::getCamera() const
{
	return this->camera;
};