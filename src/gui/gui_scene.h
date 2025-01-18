#pragma once

#include <gui/gui_item.h>

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