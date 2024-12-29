#pragma once

#include "utils/deps.h"
#include "log.h"

struct TextureImage2D {
	Log* log;

	unsigned width;
	unsigned height;
	unsigned components;
	bool inited_with_image;
	bool use_mipmaps;
	GLuint id;

	// Gen OpenGL texture object, mp - use MipMapping
	TextureImage2D(Log* logger, bool mp);
	~TextureImage2D();

	// Delete copy constructor
	TextureImage2D& operator=(TextureImage2D&) = delete;
	TextureImage2D(TextureImage2D&) = delete;
	TextureImage2D(const TextureImage2D&) = delete; 

	/*
	 *	PNG only
	 * 	Path string: $app_dir/assets/$assets_path.png
	 */
	void initFromFile(std::string assets_path); // Init bytes from file

	inline void setWrapping(GLint type);
	inline void setFiltering(GLint type);
	void setFilteringMipmap(GLint type_min, GLint type_mag);
	/*
	 * 	Sends bytes to vidmemory
	 *	UNSIGNED_BYTE only
	 *	Usage examle:
	 *		TextureImage2D::sendBytes(GL_RGBA, bytes);
	 */
	void sendBytes(GLint gl_comps, unsigned char* bytes);

	void bind();
};

inline void useTextureInSlot(GLint slot, TextureImage2D image);