#include "gui_text.h"

using namespace b_GUI;
using namespace b_Model;

namespace b_GuiFont = b_GUI::b_Font;

static unsigned gui_text_total = 0;

GUIText::GUIText (b_GuiFont::Font* f, std::string t)
{
	std::string mesh_name
		{"GUIText_" + std::to_string(gui_text_total)};
	
	// Allocate vidmemory to string vertices
	this->mesh = new BaseMesh{mesh_name.c_str()};
	// Reserved vidmemory: 16 * 6 * 256 = 24.57kb
	mesh->AddDynamicBuffer(NULL, 0, sizeof(SimpleVertex2D) * 6 * GUI_MAX_STRING_LEN);
	// Link mesh attributes
	// 0. in_position
	mesh->SetDataPointer(
		0, 2, sizeof(SimpleVertex2D), 0);
	// 1. in_texcoord
	mesh->SetDataPointer(
		0, 2, sizeof(SimpleVertex2D), offsetof(SimpleVertex2D, texcoord));
	
	this->text = t;
	this->font = f;
	this->type = GUI_TEXT;
	gui_text_total++;
	need_redraw = true;
};

void GUIText::update()
{
	if (text.length() > 0)
	{
		need_redraw = text != last_text ? true : false;
		last_text = text;
	}
};

BaseMesh* GUIText::getMesh() const
{
	return this->mesh;
};
b_Font::Font* GUIText::getFont() const
{
	return this->font;
};

GUIText::~GUIText()
{
	delete this->mesh;
};

using namespace b_Model;

void b_GUI::StringToMesh(GUIText* t)
{
	const std::string& str = t->text;
	if (str.length() > 0 && str.length() < GUI_MAX_STRING_LEN)
	{
		b_Font::Font* f = t->getFont(); // Font object
		BaseMesh* m = t->getMesh(); // Mesh object
		
		int x = 0; // 'Caret' position
		// Use -f->ascent to shift glyhs by lineheight
		int y = 0; // 'Caret' position

		// -- Glyph vertices
		SimpleVertex2D lb, rb, lt, rt; // Glyph vertices lb = left bottom and same...
		// Character - glyph position
		glm::vec2 uv_1, uv_2;
		float g_w = 0, g_h = 0; // Glyph width and height
		float g_x = 0, g_y = 0; // Glyph position

		// Character - glyph info
		unsigned lsb, aw;
		int c_width, c_height;
		int y_offset;

		unsigned index = 0;
		for (char c : str)
		{
			// 0. If it is 'Space'
			if (c == ' ')
			{
				// Add average advance width and left side bearing
				x += f->ave_lsb;
				x += f->ave_aw;
				continue;
			}
			// 0. If it is escape sequence
			if (c == '\n')
			{
				// Shift caret to lineheight
				// 5 is magic number to create some margin between lines
				y -= f->ascent + 5;
				x = 0;
				continue;
			};
			// 1. Get glyph characteristics
			b_Font::Character& ch_obj = f->char_map.at(c);
			c_width = ch_obj.getWidth();
			c_height = ch_obj.getHeight();
			lsb = ch_obj.lsb;
			aw = ch_obj.aw;
			y_offset = ch_obj.y_offset;

			x += lsb;

			// 2. Compute glyph UV position in font atlas space
			// uv_1 - Left top
			uv_1 = {
				(float)ch_obj.x1 / (float)f->atlas_w,
				// Image top left is (0, 0) Texture top left is (0, 1)
				1.f - ((float)ch_obj.y1 / (float)f->atlas_h)
			};
			// uv_2 - Bottom right
			uv_2 = {
				(float)ch_obj.x2 / (float)f->atlas_w,
				// Image top left is (0, 0) Texture top left is (0, 1)
				1.f - ((float)ch_obj.y2 / (float)f->atlas_h)
			};

			// 3. Compute glyph rect positon (atlas size relative)
			g_w = (float)c_width / (float)f->atlas_w;
			g_h = (float)c_height / (float)f->atlas_h;
			// Get bottom left point of glyph
			g_x = (float)x / (float)f->atlas_w;
			// - y_offset - different glyhs has different height
			g_y = (float)((y - c_height - y_offset)) / (float)f->atlas_h;

			// 4. Form glyph vertices
			lb = { {g_x,       g_y      }, {uv_1.x, uv_2.y}  };
			rb = { {g_x + g_w, g_y      }, {uv_2}            };
			lt = { {g_x,       g_y + g_h}, {uv_1}            };
			rt = { {g_x + g_w, g_y + g_h}, {uv_2.x, uv_1.y}  };

			// 5. Set glyph vertices to list
			t->verts[(index * 6) + 0] = rb;
			t->verts[(index * 6) + 1] = lb;
			t->verts[(index * 6) + 2] = rt;
			t->verts[(index * 6) + 3] = lb;
			t->verts[(index * 6) + 4] = lt;
			t->verts[(index * 6) + 5] = rt;

			// Shift caret at advance width
			x += aw;
			++index;
		}
		// Index - all not space characters
		size_t verts_count = index * 6; 

		// Add this data to mesh buffer object
		m->UpdateBuffer(
			0, // Buffer index
			0, // Byte offset
			verts_count * sizeof(SimpleVertex2D), // Byte size of data
			t->verts.data() // Data first element pointer
		);
		m->setTotal(verts_count);	

	}
};