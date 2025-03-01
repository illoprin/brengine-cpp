#pragma once

#include <gui/gui_font.h>
#include <gui/gui_item.h>
#include <mesh/base_mesh.h>
#include <model/model.h>

namespace b_GUI
{
	class GUIText : public GUIItem
	{
	public:
		GUIText(b_Font::Font*, std::string);
		~GUIText();

		// Compute 'need_redraw' statement
		void update() override;
		std::vector<b_Model::SimpleVertex2D> verts{6 * GUI_MAX_STRING_LEN};

		std::string text{""};

		BaseMesh* getMesh() const override;
		b_Font::Font* getFont() const override;
	private:
		std::string last_text;

		BaseMesh* mesh;
		b_Font::Font* font;
	};

	void StringToMesh(GUIText*);
};