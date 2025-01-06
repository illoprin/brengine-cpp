// Font parser, atlas builder and text rasterizer test

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_truetype.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <vector>
#include <map>
#include <string>
#include <locale.h>

const char SYMBOLS_START_INDEX = 33;
const char SYMBOLS_END_INDEX = 123;

namespace b_Font
{
	void BufferFromTTF(
		std::string      filename,
		size_t*          size,
		unsigned char**  bytes
	)
	{
		if (*bytes != NULL)
		{
			free(*bytes);
			*bytes = NULL;
		}

		std::string file_path{
			"assets/fonts/" + filename + ".ttf"
		};
		FILE* ttf_src = fopen(file_path.c_str(), "rb"); // Open TTF file in read bytes mode
		if (!ttf_src)
		{
			printf("b_Font::BufferFromTTF - Could not open file with path %s\n",
			file_path.c_str());
			return;
		}

		fseek(ttf_src, 0, SEEK_END); // Move caret in end of file
		*size = ftell(ttf_src); // Get position of caret in bytes
		fseek(ttf_src, 0, SEEK_SET); // Reset caret position

		*bytes = (unsigned char*)malloc(*size); // Allocate memory for ttf bytes
		fread(*bytes, *size, 1, ttf_src); // Read bytes from file
		fclose(ttf_src); // Close file
		
		printf("b_Font::LoadTTFBytes - Opened file %s, size of bytes is %lu\n",
			file_path.c_str(), *size);
	};

	void BuildTextBitmapBuffer(unsigned char** bitmap_buffer, unsigned w, unsigned h, int line_height, stbtt_fontinfo& info)
	{
		if (*bitmap_buffer != nullptr)
			free(*bitmap_buffer);
		*bitmap_buffer = (unsigned char*)calloc(w * h, 1);

		// em - Font unit size
		float scale_em = stbtt_ScaleForPixelHeight(&info, line_height);

		int ascent, descent, line_gap;
		stbtt_GetFontVMetrics(&info, &ascent, &descent, &line_gap);
		ascent = (int)roundf((float)ascent * scale_em); 
		descent = (int)roundf((float)descent * scale_em);

		int index = 0;
		int current_line = 1;
		int x = 0, y = 0;
		int byte_offset = 0;

		for (int i = SYMBOLS_START_INDEX; i < SYMBOLS_END_INDEX; i++)
		{
			char s = (char)i;
			// aw = advance width, lsb = left side bearing
			int aw, lsb;
			stbtt_GetCodepointHMetrics(&info, s, &aw, &lsb);

			// Scale horizontal parametres at em unit
			aw = (int)roundf(aw * scale_em);
			lsb = (int)roundf(lsb * scale_em);
			
			// Shift x caret at left bearing
			x += lsb;
			
			// Check glyph out of bounds
			if (x + aw > w)
			{
				++current_line;
				x = lsb;
			}

			// Y - is sum of current line ascents
			// Ascent in this case is (top shift of glyph + base line height)
			// We add a half of line height to give some vertical space for glyps
			y = (ascent) * current_line + (current_line - 1)  * (line_height / 2);
			if (y > h)
				break;

			// Get bounding box of glyph
			int x1, y1, x2, y2;
			stbtt_GetCodepointBitmapBox(&info, s, scale_em, scale_em, &x1, &y1, &x2, &y2);

			// (y + y1) - different glyphs has different top shift
			byte_offset = x + (y + y1) * w;
			// bitmap_buffer[x + (y + y1) * bitmap_w] = 160; // Add pixel at origin of glyph
			stbtt_MakeCodepointBitmap(&info, *bitmap_buffer + byte_offset, x2 - x1, y2 - y1, w, scale_em, scale_em, s);

			// Shift caret at adwance width
			x += aw;

			// Kern - shift between previous and next glyph
			int kern = 0;
			if (i + 1 <= SYMBOLS_END_INDEX)
				kern = stbtt_GetCodepointKernAdvance(&info, i, i + 1);
			x += (int)roundf(kern * scale_em);

			index++;
		};
	};

	struct Character
	{
		// bounding box in atlas space
		unsigned x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		int y_offset = 0;
		int lsb = 0, aw = 0;

		inline unsigned getWidth() const
		{
			return this->x2 - this->x1;
		};
		inline unsigned getHeight() const
		{
			return this->y2 - this->y1;
		}

	};


	struct Font
	{
		Font(std::string name, int lineheight)
		{
			ttf_buffer = NULL;
			size_t ttf_size;
			this->name = name;
			BufferFromTTF(name, &ttf_size, &ttf_buffer);

			if (stbtt_InitFont(&this->info, ttf_buffer, 0))
			{
				printf("Font %s - Inited\n", name.c_str());
			};
			this->scale_em = stbtt_ScaleForPixelHeight(&info, lineheight);

			int descent, line_gap;
			stbtt_GetFontVMetrics(&this->info, &this->ascent, &descent, &line_gap);

			this->ascent = (int)roundf(this->ascent * scale_em);
		};

		void buildAtlas(unsigned w, unsigned h)
		{
			// TODO: Load cached atlas if exists
			this->atlas = (unsigned char*)calloc(w * h, 1);
			int current_line = 1;
			int x = 0, y = 0;
			int byte_offset;
			for (char i = SYMBOLS_START_INDEX; i <= SYMBOLS_END_INDEX; i++)
			{
				// aw = advance width, lsb = left side bearing
				int aw, lsb;
				stbtt_GetCodepointHMetrics(&info, i, &aw, &lsb);

				// Scale horizontal parametres at em unit
				aw = (int)roundf(aw * scale_em);
				lsb = (int)roundf(lsb * scale_em);
				
				// Shift x caret at left bearing
				x += lsb;
				
				// Check glyph out of bounds
				if (x + aw > w)
				{
					++current_line;
					x = lsb;
				}

				// Y - is sum of current line ascents
				// Ascent in this case is (top shift of glyph + base line height)
				// We add a half of line height to give some vertical space for glyps
				y = (ascent) * current_line + (current_line - 1) * 10; // 10 is magic number for line padding
				if (y > h)
					break;

				// Get bounding box of glyph
				int x1, y1, x2, y2;
				stbtt_GetCodepointBitmapBox(&info, i, this->scale_em, this->scale_em, &x1, &y1, &x2, &y2);

				// (y + y1) - different glyphs has different top shift
				byte_offset = x + (y + y1) * w;
				// bitmap_buffer[x + (y + y1) * bitmap_w] = 160; // Add pixel at origin of glyph
				stbtt_MakeCodepointBitmap(&info, this->atlas + byte_offset, x2 - x1, y2 - y1, w, this->scale_em, this->scale_em, i);
				
				Character c;
				c.x1 = (unsigned)(x);
				c.y1 = (unsigned)(y + y1);
				c.x2 = (unsigned)(x + x1 + (x2 - x1));
				c.y2 = (unsigned)(y + y1 + (y2 - y1));
				c.y_offset = y1;
				c.aw = aw;
				c.lsb = lsb;
				this->char_map[(char)i] = c;

				ave_aw += aw;
				ave_lsb += lsb;


				// Shift caret at adwance width
				x += aw + 5; // 5 - magic number for padding				
			};
			ave_aw = (int)round((double)ave_aw / (double)(SYMBOLS_END_INDEX - SYMBOLS_START_INDEX));
			ave_lsb = (int)round((double)ave_lsb / (double)(SYMBOLS_END_INDEX - SYMBOLS_START_INDEX));

			// Write created atlas to image
			std::string atlas_filename{"tmp/" + this->name + "_atlas.png"}; 
			if (stbi_write_png(atlas_filename.c_str(), w, h, 1, this->atlas, w))
			{
				printf("Font %s - Atlas writed\n", name.c_str());
			}
			this->atlas_w = w;
			this->atlas_h = h;
		};

		void printMap()
		{
			for (const auto& p : this->char_map)
			{
				printf("Character: %c; Width: %u; Height: %u; X1: %u; Y1: %u; X2: %u; Y2: %u; YOffset: %d\n",
					p.first, p.second.getWidth(), p.second.getHeight(), p.second.x1, p.second.y1, p.second.x2, p.second.y2, p.second.y_offset);
			};
		};

		void processString(std::string str, int w, int h)
		{
			if (this->atlas == nullptr)
			{
				printf("Font %s - Build atlas before process\n", name.c_str());
				return;
			}
			unsigned char* image_buffer = (unsigned char*)calloc(w * h, 1);
			
			int x = 0, y = this->ascent; // Image 'caret' position
			for (char c : str)
			{
				if (c == ' ')
				{
					x += (ave_aw + ave_lsb);
					continue;
				}
				Character& ch_obj = this->char_map.at(c);
				int advance_width = ch_obj.aw;
				int lsb = ch_obj.lsb;
				int c_w = ch_obj.getWidth();
				int c_h = ch_obj.getHeight();

				// Blit atlas bytes to image
				x += lsb;
				int atlas_bo = 0, img_bo = 0; // bo = byte offset
				int r_x, r_y; // Relative position of iteration
				for (int b = 0; b < (c_w * c_h); b++)
				{
					r_x = b % c_w;
					r_y = (int)floor((double)b / (double)c_w);
					
					atlas_bo = (ch_obj.x1 + r_x) + (ch_obj.y1 + r_y) * this->atlas_w;
					img_bo = (x + r_x) + (y + r_y + ch_obj.y_offset) * w;
					image_buffer[img_bo] = this->atlas[atlas_bo];
				};
				x += advance_width;
			}

			std::string filename{"tmp/string_" + str + "_atlas_" + this->name + ".png"};
			if (stbi_write_png(filename.c_str(), w, h, 1, image_buffer, w))
			{
				printf("String %s processed, image %s writed\n",
					str.c_str(), filename.c_str());
			}

			free(image_buffer);
		};

		~Font()
		{
			if (ttf_buffer) free(ttf_buffer);
			if (atlas) free(atlas);
			printf("Font %s - Released\n", this->name.c_str());
		};

		stbtt_fontinfo info;
		std::map<char, Character> char_map;
		int ascent;
		float scale_em;
		std::string name;
		unsigned char* ttf_buffer;

		int ave_aw = 0; // Average advance width, needs for spaceing
		int ave_lsb = 0; // Average left side bearing, needs for spaceing
		unsigned char* atlas;
		unsigned atlas_w, atlas_h;
	};
};

const char* char_set =
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Pellentesque varius, orci eget faucibus finibus, turpis turpis cursus nulla, sit amet iaculis nibh felis a ligula. In consequat massa id pulvinar volutpat. Proin elit orci, luctus eu volutpat ligula.";
//"ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя:-()~[]\"'";

int main()
{
	setlocale(LC_ALL, "");
	b_Font::Font* f = new b_Font::Font{"mono", 64}; // test mono font
	f->buildAtlas(512, 512);
	f->processString("hell shit :)", 512, 512);
	delete f;

	b_Font::Font* s = new b_Font::Font{"sans", 32}; // test sans font
	s->buildAtlas(512, 512);
	s->processString("hello world UwU", 512, 512);
	delete s;
};
