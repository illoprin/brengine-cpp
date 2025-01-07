#pragma once

#include "../utils/deps.h"
#include "gui_item.h"


namespace b_GUI
{
	class GUIImage : public GUIItem
	{
	public:
		GUIImage();
		GUIImage(TextureImage2D* t);
		~GUIImage() = default;
		
		TextureImage2D* getTexture() const override;
		void setTexture(TextureImage2D*);
	private:
		TextureImage2D* texture;
	};
};


