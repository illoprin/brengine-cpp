#pragma once

#include "../utils/deps.h"
#include "gui_item.h"


namespace b_GUI
{
	class GUIImage : public GUIItem
	{
	public:
		GUIImage();
		GUIImage(b_Texture::TextureImage2D* t);
		~GUIImage() = default;
		
		b_Texture::TextureImage2D* getTexture() const override;
		void setTexture(b_Texture::TextureImage2D*);
	private:
		b_Texture::TextureImage2D* texture;
	};
};


