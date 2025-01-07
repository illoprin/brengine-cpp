#pragma once

#include "../utils/deps.h"
#include "../mesh/base_mesh.h"
#include "../render/texture.h"
#include "gui_font.h"

namespace b_GUI
{
	class GUIItem
	{
	public:
		friend class GUIScene;
		GUIItem();
		~GUIItem();

		// Delete copy constructors
		GUIItem& operator=(GUIItem&) = delete;
		GUIItem(const GUIItem&) = delete;
		GUIItem(GUIItem&) = delete;

		// For override
		virtual TextureImage2D* getTexture() const;
		virtual BaseMesh* getMesh() const;
		virtual b_Font::Font* getFont() const;
		virtual void update();

		float getRotation();
		const glm::vec2& getPosition();
		const glm::vec2& getScaling();
		const glm::vec4& getColor();
		GUIItemType getType();

		bool need_redraw = true;

		void setPosition(glm::vec2);
		void setScaling(glm::vec2);
		void setRotation(float);
		void setColor(glm::vec3);
		void setAlpha(float);
	protected:
		glm::vec2 position{0.f, 0.f};
		glm::vec2 scaling{1.f, 1.f};
		float rotation = 0.f;
		glm::vec4 color{1.f, 1.f, 1.f, 1.f};
		GUIItemType type;
	};
}