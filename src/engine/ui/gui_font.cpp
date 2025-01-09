#include "gui_font.h"

#include "../engine.h"

using namespace b_GUI;

// ASCII English symbols index range
const char SYMBOLS_START_INDEX = 33;
const char SYMBOLS_END_INDEX = 123;

void b_Font::BufferFromTTF(
	const char*      fp,
	unsigned char**  buffer,
	size_t*          size
)
{
	if (*buffer != NULL)
	{
		free(*buffer);
		*buffer = NULL;
	}

	FILE* ttf_src = fopen(fp, "rb"); // Open TTF file in read bytes mode
	if (!ttf_src)
	{
		printf("b_Font::BufferFromTTF - Could not open file with path %s\n", fp);
		return;
	}

	fseek(ttf_src, 0, SEEK_END); // Move caret in end of file
	*size = ftell(ttf_src); // Get position of caret in bytes
	fseek(ttf_src, 0, SEEK_SET); // Reset caret position

	*buffer = (unsigned char*)malloc(*size); // Allocate memory for ttf bytes
	fread(*buffer, *size, 1, ttf_src); // Read bytes from file
	fclose(ttf_src); // Close file
	
	printf("b_Font::LoadTTFBytes - Opened file %s, size of bytes is %lu\n", fp, *size);
};

/* ============ FONT ============ */

b_Font::Font::Font(std::string n)
{
	this->name = n;

	// Create texture object for atlas
	this->t_atlas = new TextureImage2D{false};
};

void b_Font::Font::configureTexture()
{
	// Configuring texture object
	this->t_atlas->components = 1;
	this->t_atlas->height = this->atlas_h;
	this->t_atlas->width = this->atlas_w;

	this->t_atlas->setFiltering(GL_LINEAR);
	this->t_atlas->setWrapping(GL_CLAMP_TO_EDGE);

	b_ImageIO::FlipY(this->atlas, 1, this->atlas_w, this->atlas_h);

	// Add atlas texture to OpenGL
	this->t_atlas->setImagePointer(
		// Red 8 bits: GL_R8
		// One red component: GL_RED
		// Data format is unsigned char: GL_UNSIGNED_BYTE
		GL_R8, GL_RED, GL_UNSIGNED_BYTE, this->atlas
	);
};

void b_Font::Font::FromTTF(
	std::string  filename,
	int          lineheight,
	unsigned     width,
	unsigned     height
)
{
	this->line_height = lineheight;
	this->atlas_w = width;
	this->atlas_h = height;

	std::string cache_filename = getCacheFileName();
	// Load font data from cache 
	// if file exists and not outdated
	if (fs::exists(cache_filename))
		if (this->loadFromCache(cache_filename.c_str()))
		{
			this->configureTexture();
			return;
		}
	
	this->info = (stbtt_fontinfo*)calloc(sizeof(stbtt_fontinfo), 1);

	if (this->info == nullptr)
	{
		b_log->logf("[WARNING] Font %s - Could not allocate memory for stbtt_fontinfo struct\n",
			this->name.c_str());
		return;		
	}

	std::string filepath{
		"assets/fonts/" + filename + ".ttf"
	};
	this->ttf_buffer = NULL; size_t ttf_size;
	BufferFromTTF(filepath.c_str(), &ttf_buffer, &ttf_size);

	if (!stbtt_InitFont(this->info, ttf_buffer, 0))
	{
		this->cleanUp();
		b_log->logf("[WARNING] Font %s - Could not init from file %s\n",
			this->name.c_str(), filename.c_str());
		return;
	}
	int line_gap;
	stbtt_GetFontVMetrics(this->info, &this->ascent, &this->descent, &line_gap);

	this->scale_em = stbtt_ScaleForPixelHeight(info, this->line_height);
	this->ascent = (int)roundf(this->ascent * scale_em);
	this->descent = (int)roundf(this->descent * scale_em);

	b_log->logf("[INFO] Font %s - Inited\n", name.c_str());

	if (this->buildAtlas(width, height))
	{
		this->cleanUp();
		this->cacheData(cache_filename.c_str());
		this->configureTexture();
	}
};

void b_Font::Font::cleanUp()
{
	if (this->ttf_buffer) free(this->ttf_buffer);
	if (this->info) free(this->info);
}

bool b_Font::Font::buildAtlas(unsigned w, unsigned h)
{
	this->atlas = (unsigned char*)calloc(
		this->atlas_w * this->atlas_h,
		sizeof(unsigned char)
	);
	if (this->atlas == nullptr)
	{
		b_log->logf("[WARNING] Font %s - Could not allocate memory for atlas\n",
			this->name.c_str());
		return false;
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

	b_log->logf("[INFO] Font %s - Atlas with width %u height %u builded\n", name.c_str(), this->atlas_w, this->atlas_h);
	return true;
};

std::string b_Font::Font::getCacheFileName()
{
	return {"tmp/" + this->name + ".bfc"};
};

void b_Font::Font::cacheData(const char* filename)
{
	// BFC Format - Brengine font cache
	// Open binary file to writing
	FILE* file = fopen(filename, "wb");
	if (!file)
	{
		b_log->logf("[WARNING] Font %s - Could not create cache file\n",
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

	b_log->logf("[INFO] Font %s - Cached to file %s\n",
		this->name.c_str(), filename);

	// Close file
	fclose(file);
};

// Return false if file is outdated, true if all data is correct
bool b_Font::Font::loadFromCache(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (!file)
	{
		b_log->logf("[WARNING] Font %s - Could not open cache file\n",
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
		b_log->logf("[INFO] Font %s - Could not process cache file with path %s\n",
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
		b_log->logf("[INFO] Font %s - Could not process cache file with path %s\n",
			this->name.c_str(), filename);
		this->char_map.clear();
		fclose(file);
		return false;
	}

	size_t pixels_total = a_w * a_h;
	unsigned char* new_atlas = (unsigned char*)malloc(pixels_total);
	b_log->logf("[INFO] Font %s - Atlas width is %u height is %u\n",
			this->name.c_str(), a_w, a_h);
	
	fread(new_atlas, sizeof(unsigned char), pixels_total, file);
	
	if (this->atlas != nullptr)
		free(this->atlas);
	this->atlas = new_atlas;
	
	b_log->logf("[INFO] Font %s - Loaded from cache\n", this->name.c_str());

	return true;
};

b_Font::Font::~Font()
{
	// Release atlas byte data
	if (this->atlas) free(this->atlas);
	// Release OpenGL texture object
	delete this->t_atlas;
	b_log->logf("[INFO] Font %s - Released\n", this->name.c_str());
};