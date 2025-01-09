#pragma once

#include "../utils/deps.h"
#include "../render/texture.h"
#include "../utils/utils.h"


/*
	BFC - Brengine Font Cache Format
	Data:
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

namespace b_GUI
{
	namespace b_Font
	{
		void BufferFromTTF(
			const char*      fp,
			unsigned char**  buffer,
			size_t*          size
		);

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
			// Average advance width, needs for spaceing
			int ave_aw = 0;
			// Average left side bearing, needs for spaceing
			int ave_lsb = 0;

			unsigned char* atlas = nullptr;
			unsigned atlas_w = 0, atlas_h = 0;

			b_Texture::TextureImage2D* t_atlas;

			Font(std::string name);
			~Font();

			/*
				Creates texture atlas based on ttf info.
				Saves info about characters to map.

				Or loads font info from cached file tmp/$fontname.bfc
			*/
			void FromTTF(
				std::string filename,
				int         lineheight,
				unsigned    width,
				unsigned    height
			);
			
			void printCharacterMap();
		private:
			void cleanUp();
			std::string getCacheFileName();
			
			bool buildAtlas(unsigned, unsigned);
			void configureTexture();

			void cacheData(const char*);
			bool loadFromCache(const char*);
		};
	};
}
