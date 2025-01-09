#include "gui_scene.h"

#include "../engine.h"

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
