#pragma once

#include "../mesh/base_mesh.h"
#include "../utils/deps.h"
#include "gui_item.h"

namespace b_GUI
{
	class GUIScene
	{
	public:
		GUIScene();
		~GUIScene() = default;
		
		// Delete copy operators
		GUIScene& operator=(GUIScene&) = delete;
		GUIScene(const GUIScene&) = delete;
		GUIScene(GUIScene&) = delete;

		void update(glm::ivec2& vid_mode);
		void append(GUIItem*);

		const std::vector<GUIItem *>& getItems() const;

		const glm::mat4& getProjection() const;
	private:
		glm::mat4 projection;
		std::vector<GUIItem *> items;
	};
};