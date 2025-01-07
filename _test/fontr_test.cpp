// Font parser, atlas builder and text rasterizer test

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_TRUETYPE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_truetype.h>

#include <locale.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <vector>
#include <map>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

const char SYMBOLS_START_INDEX = 33;
const char SYMBOLS_END_INDEX = 126;

namespace b_Font
{
	void BufferFromTTF(
		const char*      file_path,
		size_t*          size,
		unsigned char**  bytes
	)
	{
		if (*bytes != NULL)
		{
			free(*bytes);
			*bytes = NULL;
		}

		FILE* ttf_src = fopen(file_path, "rb"); // Open TTF file in read bytes mode
		if (!ttf_src)
		{
			printf("b_Font::BufferFromTTF - Could not open file with path %s\n",
			file_path);
			return;
		}

		fseek(ttf_src, 0, SEEK_END); // Move caret in end of file
		*size = ftell(ttf_src); // Get position of caret in bytes
		fseek(ttf_src, 0, SEEK_SET); // Reset caret position

		*bytes = (unsigned char*)malloc(*size); // Allocate memory for ttf bytes
		fread(*bytes, *size, 1, ttf_src); // Read bytes from file
		fclose(ttf_src); // Close file
		
		printf("b_Font::LoadTTFBytes - Opened file %s, size of bytes is %lu\n",
			file_path, *size);
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
	public:
		std::string name;
		stbtt_fontinfo* info = nullptr;
		unsigned char* ttf_buffer;
		
		int line_height = 0, ascent = 0, descent = 0;
		float scale_em = 0.f;

		std::map<char, Character> char_map;
		int ave_aw = 0; // Average advance width, needs for spaceing
		int ave_lsb = 0; // Average left side bearing, needs for spaceing
		unsigned char* atlas = nullptr;
		unsigned atlas_w = 0, atlas_h = 0;

		Font(std::string n) : name(n) {};

		void FromTTF(std::string filename, int lineheight, unsigned width, unsigned height)
		{
			this->line_height = lineheight;
			this->atlas_w = width;
			this->atlas_h = height;
		
			std::string cache_filename = getCacheFileName();
			// Load font data from cache 
			// if file exists and not outdated
			if (fs::exists(cache_filename))
				if (this->loadFromCache(cache_filename.c_str()))
					return;
			
			this->info = (stbtt_fontinfo*)calloc(sizeof(stbtt_fontinfo), 1);


			std::string filepath{
				"assets/fonts/" + filename + ".ttf"
			};
			this->ttf_buffer = NULL; size_t ttf_size;
			BufferFromTTF(filepath.c_str(), &ttf_size, &ttf_buffer);

			if (!stbtt_InitFont(this->info, ttf_buffer, 0))
			{
				this->cleanUp();
				return;
			}
			int line_gap;
			stbtt_GetFontVMetrics(this->info, &this->ascent, &this->descent, &line_gap);

			this->scale_em = stbtt_ScaleForPixelHeight(info, this->line_height);
			this->ascent = (int)roundf(this->ascent * scale_em);
			this->descent = (int)roundf(this->descent * scale_em);

			printf("Font %s - Inited\n", name.c_str());

			this->buildAtlas(width, height);
			this->cleanUp();
			this->cacheData(cache_filename.c_str());
		}

		void buildAtlas(unsigned w, unsigned h)
		{			
			this->atlas = (unsigned char*)calloc(this->atlas_w * this->atlas_h, sizeof(unsigned char));
			if (this->atlas == nullptr)
			{
				fprintf(stderr, "Font %s - Could not allocate memory for atlas\n",
					this->name.c_str());
				return;
			}
			int x = 0, y = 0, current_line = 1;
			int byte_offset;
			for (char i = SYMBOLS_START_INDEX; i < SYMBOLS_END_INDEX; i++)
			{
				// aw = advance width, lsb = left side bearing
				int aw, lsb;
				stbtt_GetCodepointHMetrics(info, i, &aw, &lsb);

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
				y = (this->ascent) * current_line + (current_line - 1) * 10; // 10 - magic number for line margin
				if (y > h)
					break;

				// Get bounding box of glyph
				int x1, y1, x2, y2;
				stbtt_GetCodepointBitmapBox(info, i, this->scale_em, this->scale_em, &x1, &y1, &x2, &y2);

				// (y + y1) - different glyphs has different top shift
				byte_offset = x + (y + y1) * w;
				stbtt_MakeCodepointBitmap(info, this->atlas + byte_offset, x2 - x1, y2 - y1, w, this->scale_em, this->scale_em, i);

				// Add character info to map				
				Character c; // This struct stores information about symbol
				c.x1 = (unsigned)x;
				c.y1 = (unsigned)(y + y1);
				c.x2 = (unsigned)(x + x1 + (x2 - x1));
				c.y2 = (unsigned)(y + y1 + (y2 - y1));
				c.y_offset = y1;
				c.aw = aw;
				c.lsb = lsb;
				
				this->char_map[(char)i] = c; // Add to map

				this->ave_aw += aw; // Add this char advance width to sum
				this->ave_lsb += lsb; // Add this char left side bearing to sum

				// Shift caret at adwance width
				x += aw + 5; // 5 - magic number for char margin			
			};
			// Calculate average advance width
			this->ave_aw =
				(int)round((double)ave_aw / (double)(SYMBOLS_END_INDEX - SYMBOLS_START_INDEX));
			// Calculate average left side bearing
			this->ave_lsb =
				(int)round((double)ave_lsb / (double)(SYMBOLS_END_INDEX - SYMBOLS_START_INDEX));

			printf("Font %s - Atlas builded\n", name.c_str());

#ifdef B_FONT_WRITE_ATLAS
			// Write created atlas to image
			std::string atlas_filename{"tmp/" + this->name + "_atlas.png"}; 
			if (stbi_write_png(atlas_filename.c_str(), w, h, 1, this->atlas, w))
			{
				printf("Font %s - Atlas writed to image\n", name.c_str());
			}
#endif
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
				// Skip space symbol
				if (c == ' ')
				{
					x += (ave_lsb + ave_aw);
					continue;
				}

				Character& ch_obj = this->char_map.at(c);
				unsigned advance_width = ch_obj.aw;
				unsigned lsb = ch_obj.lsb;
				unsigned c_w = ch_obj.getWidth();
				unsigned c_h = ch_obj.getHeight();

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

			std::string filename{"tmp/str_" + str + "_font_" + this->name + ".png"};
			if (stbi_write_png(filename.c_str(), w, h, 1, image_buffer, w))
			{
				printf("String %s processed, image %s writed\n",
					str.c_str(), filename.c_str());
			}

			free(image_buffer);
		};

		~Font()
		{
			if (atlas) free(atlas);
			printf("Font %s - Released\n", this->name.c_str());
		};

	private:
		std::string getCacheFileName()
		{
			return {"tmp/" + this->name + ".bfc"};
		};
		void cleanUp()
		{
			if (this->ttf_buffer) free(this->ttf_buffer);
			if (this->info) free(this->info);
		};

		/*
			Creates font cache binary file with format:
				uint16_t line_height
				uint16_t number_of_characters

				uint8_t symbols_start_ascii
				uint8_t symbols_end_ascii
				times - (number_of_characters) {
					uint32_t x1
					uint32_t y1
					uint32_t x2
					uint32_t y2
					int32_t y_offset
					int32_t lsb 
					int32_t aw 
				};

				uint16_t average_left_side_bearing
				uint16_t average_advance_width

				uint16_t atlas_width
				uint16_t atlas_height

				times - (atlas_width * atlas_height) {
					unsigned char pixel
				}
		*/
		void cacheData(const char* filename)
		{
			// BFC Format - Brengine font cache
			// Open binary file to writing
			FILE* file = fopen(filename, "wb");
			if (!file)
			{
				fprintf(stderr, "Font %s - Could not create cache file\n",
					this->name.c_str());
				return;
			}

			uint16_t lh = (uint16_t)this->line_height;
			fwrite(&lh, sizeof(uint16_t), 1, file);

			int32_t asc = (int32_t)this->ascent;
			fwrite(&asc, sizeof(int32_t), 1, file);

			int32_t dsc = (int32_t)this->descent;
			fwrite(&dsc, sizeof(int32_t), 1, file);
			
			uint16_t num_chars = (uint16_t)this->char_map.size();
			fwrite(&num_chars, sizeof(uint16_t), 1, file);
			
			uint8_t start_ascii = (uint8_t)SYMBOLS_START_INDEX;
			fwrite(&start_ascii, sizeof(uint8_t), 1, file);

			uint8_t end_ascii = (uint8_t)SYMBOLS_END_INDEX;
			fwrite(&end_ascii, sizeof(uint8_t), 1, file);
			
			for (const auto& pair : this->char_map)
			{
				const Character& ch = pair.second;
				uint32_t x1 = (uint32_t)ch.x1;
				fwrite(&x1, sizeof(uint32_t), 1, file);
				uint32_t y1 = (uint32_t)ch.y1;
				fwrite(&y1, sizeof(uint32_t), 1, file);
				uint32_t x2 = (uint32_t)ch.x2;
				fwrite(&x2, sizeof(uint32_t), 1, file);
				uint32_t y2 = (uint32_t)ch.y2;
				fwrite(&y2, sizeof(uint32_t), 1, file);

				int32_t y_of = (int32_t)ch.y_offset;
				fwrite(&y_of, sizeof(int32_t), 1, file);
				int32_t lsb = (int32_t)ch.lsb;
				fwrite(&lsb, sizeof(int32_t), 1, file);
				int32_t aw = (int32_t)ch.aw;
				fwrite(&aw, sizeof(int32_t), 1, file);
			};
			uint16_t a_lsb = (uint16_t)this->ave_lsb;
			fwrite(&a_lsb, sizeof(uint16_t), 1, file);

			uint16_t a_aw = (uint16_t)this->ave_aw;
			fwrite(&a_aw, sizeof(uint16_t), 1, file);

			uint16_t a_w = (uint16_t)this->atlas_w;
			fwrite(&a_w, sizeof(uint16_t), 1, file);
			uint16_t a_h = (uint16_t)this->atlas_h;
			fwrite(&a_h, sizeof(uint16_t), 1, file);

			size_t pixels_total = this->atlas_w * this->atlas_h;
			fwrite(this->atlas, sizeof(unsigned char), pixels_total, file);

			printf("Font %s - Cached to file %s\n",
				this->name.c_str(), filename);

			// Close file
			fclose(file);
		};

		// Returns 1 if cache file is outdated
		bool loadFromCache(const char* filename)
		{
			FILE* file = fopen(filename, "rb");
			if (!file)
			{
				fprintf(stderr, "Font %s - Could not open cache file\n",
					this->name.c_str());
				return false;
			}
			// Read line height
			uint16_t lh;
			fread(&lh, sizeof(uint16_t), 1, file);
			if ((int)lh != this->line_height)
			{
				printf("Font %s - Could not process cache file with path %s\n",
					this->name.c_str(), filename);
				fclose(file);
				return false;
			};

			int32_t asc;
			fread(&asc, sizeof(int32_t), 1, file);
			this->ascent = (int)asc;
			
			int32_t dsc;
			fread(&dsc, sizeof(int32_t), 1, file);
			this->descent = (int)dsc;

			uint16_t num_char;
			fread(&num_char, sizeof(uint16_t), 1, file);
			if (num_char != (uint16_t)(SYMBOLS_END_INDEX - SYMBOLS_START_INDEX))
			{
				printf("Font %s - Could not process cache file with path %s\n",
					this->name.c_str(), filename);
				fclose(file);
				return false;
			}
			
			fseek(file, 2L, SEEK_CUR); // Skip 2 bytes
			
			this->char_map.clear();
			for (uint16_t i = 0; i < num_char; i++)
			{
				Character c; // This struct stores information about symbol

				uint32_t x1;
				fread(&x1, sizeof(uint32_t), 1, file);
				uint32_t y1;
				fread(&y1, sizeof(uint32_t), 1, file);
				uint32_t x2;
				fread(&x2, sizeof(uint32_t), 1, file);
				uint32_t y2;
				fread(&y2, sizeof(uint32_t), 1, file);

				int32_t y_offset;
				fread(&y_offset, sizeof(int32_t), 1, file);
				int32_t lsb;
				fread(&lsb, sizeof(int32_t), 1, file);
				int32_t aw;
				fread(&aw, sizeof(int32_t), 1, file);

				// Add character info to map				
				c.x1 = (unsigned)x1;
				c.y1 = (unsigned)y1;
				c.x2 = (unsigned)x2;
				c.y2 = (unsigned)y2;
				c.y_offset = (int)y_offset;
				c.aw = (int)aw;
				c.lsb = (int)lsb;
				
				this->char_map[(char)(SYMBOLS_START_INDEX + i)] = c;
			};

			uint16_t a_lsb;
			fread(&a_lsb, sizeof(uint16_t), 1, file);
			this->ave_lsb = (int)a_lsb;
			
			uint16_t a_aw;
			fread(&a_aw, sizeof(uint16_t), 1, file);
			this->ave_aw = (int)a_aw;

			uint16_t a_w; // Atlas width
			fread(&a_w, sizeof(uint16_t), 1, file);

			uint16_t a_h; // Atlas height
			fread(&a_h, sizeof(uint16_t), 1, file);

			if (this->atlas_w != (unsigned)a_w || this->atlas_h != (unsigned)a_h)
			{
				printf("Font %s - Could not process cache file with path %s\n",
					this->name.c_str(), filename);
				this->char_map.clear();
				fclose(file);
				return false;
			}

			size_t pixels_total = a_w * a_h;
			unsigned char* new_atlas = (unsigned char*)malloc(pixels_total);
			printf("Font %s - Atlas width is %u height is %u\n",
					this->name.c_str(), a_w, a_h);
			
			fread(new_atlas, sizeof(unsigned char), pixels_total, file);
			
			if (this->atlas != NULL)
				free(this->atlas);
			this->atlas = new_atlas;
			printf("Font %s - Loaded from cache\n", this->name.c_str());

			return true;
		};
	};
};


//"ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyzАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзийклмнопрстуфхцчшщъыьэюя:-()~[]\"'";

int main()
{
	setlocale(LC_ALL, "");
	b_Font::Font* f = new b_Font::Font{"MonoFont"}; // test mono font
	f->FromTTF("mono", 32, 1024, 1024);
	f->printMap();
	f->processString("UwU your ad here && :)(( ::))", 1024, 200);
	delete f;

	b_Font::Font* s = new b_Font::Font{"SansFont"}; // test sans font
	s->FromTTF("sans", 32, 512, 512);
	s->processString("hello world || :)", 256, 256);
	delete s;
};
