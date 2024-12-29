#include "editor_controller.h"

EditorController::EditorController(Engine* e)
{
	this->engine = e;
	this->camera = new Camera();
};

EditorController::~EditorController()
{
	delete this->camera;
};

void EditorController::update()
{
	this->update_states();
	if (engine->isGameMode())
	{
		this->handle_key_events();
		this->handle_mouse_events();
	}
};
void EditorController::update_states()
{
	this->states[PL_M_FORWARD] = glfwGetKey(engine->getWindow(), GLFW_KEY_W);
	this->states[PL_M_BACKWARD] = glfwGetKey(engine->getWindow(), GLFW_KEY_S);
	this->states[PL_M_LEFT] = glfwGetKey(engine->getWindow(), GLFW_KEY_A);
	this->states[PL_M_RIGHT] = glfwGetKey(engine->getWindow(), GLFW_KEY_D);
	this->states[PL_M_MODIFY] = glfwGetKey(engine->getWindow(), GLFW_KEY_LEFT_SHIFT);
};
void EditorController::handle_key_events()
{
	float vel = engine->getClock()->getDeltaTime() * PLAYER_SPEED;

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
	float factor = PLAYER_SENSITIVITY * engine->getClock()->getDeltaTime();
	
	this->camera->rotate(
		(float)-engine->getIO()->getMouse()->m_delta_y * factor,
		(float)engine->getIO()->getMouse()->m_delta_x * factor
	);
};

Camera* EditorController::getCamera() const
{
	return this->camera;
};