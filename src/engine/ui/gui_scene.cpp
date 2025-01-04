#include "gui_scene.h"

using namespace b_GUI;

GUIScene::GUIScene()
{

};

void GUIScene::append(GUIItem* i)
{
	this->items.push_back(i);
};

const std::vector<GUIItem *>& GUIScene::getItems() const
{
	return this->items;
};

const glm::mat4& GUIScene::getProjection() const
{
	return this->projection;
};

void GUIScene::update(glm::ivec2& vid_mode)
{
	float aspect = (float)vid_mode.x / (float)vid_mode.y;
	this->projection = glm::ortho(
		-aspect, aspect, // X
		-1.f, 1.f, // Y
		0.f, 1.f // Near Far
	);
}