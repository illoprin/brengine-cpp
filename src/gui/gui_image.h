#pragma once

#include <gui/gui_item.h>

namespace b_GUI
{
	class GUIImage : public GUIItem
	{
	public:
		GUIImage();
		GUIImage(b_Texture::Texture* t);
		~GUIImage() = default;
		
		b_Texture::Texture* getTexture() const override;
		void setTexture(b_Texture::Texture*);
	private:
		b_Texture::Texture* texture;
	};
};


