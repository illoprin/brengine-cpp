#pragma once

#include "gui_item.h"

namespace b_GUI
{
	class GUIText : public GUIItem
	{
	public:
		GUIText(std::string);
		~GUIText();

		const std::string& getText() const;
	private:
		unsigned font_size;
		std::string text;
	};
};