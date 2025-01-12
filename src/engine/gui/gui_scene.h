#pragma once

#include "../mesh/base_mesh.h"
#include "../util/deps.h"
#include "gui_item.h"
#include "gui_text.h"

namespace b_GUI
{
	class GUIScene
	{
	public:	
		GUIScene() = default;

		// Delete copy operators
		GUIScene& operator=(GUIScene&) = delete;
		GUIScene(const GUIScene&) = delete;
		GUIScene(GUIScene&) = delete;
		void append(GUIItem*);
		const std::vector<GUIItem *>& getItems() const;
	private:
		std::vector<GUIItem *> items;
	};
};