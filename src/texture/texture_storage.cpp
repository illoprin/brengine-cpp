#include "texture_storage.h"

#include "../core/engine.h"
#include "../core/context.h"

using namespace b_Texture;

TextureStorage3D::TextureStorage3D()
{
	b_log->logf("[INFO] TextureStorage id = %u - Generated\n", id);
};
void TextureStorage3D::bind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, id);
};

void TextureStorage3D::InitStorage(GLuint internalFormat)
{
	bind();
	glTexStorage3D(GL_TEXTURE_2D_ARRAY,
		1, internalFormat, this->width, this->height, this->layers);
	b_log->logf("[INFO] TextureStorage id = %u - Inited with width %u height %u layer count is %u\n", id, width, height, layers);
	b_CheckError(); // Checking OpenGL errors
};

void TextureStorage3D::setImagePointer(
	unsigned index, unsigned ox, unsigned oy, GLuint format, GLuint data_type, void* pixels
)
{
	bind();
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,
		0, ox, oy, index, width, height, 1, format, data_type, pixels);
	b_log->logf("[INFO] TextureStorage id = %u - Created image pointer to layer %u\n", id, index);
	b_CheckError(); // Checking OpenGL errors
};

void TextureStorage3D::setWrapping(GLint t)
{
	bind();
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, t);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, t);
};
void TextureStorage3D::setFiltering(GLint t)
{
	bind();
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, t);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, t);
};

TextureStorage3D::~TextureStorage3D()
{
	b_log->logf("[INFO] TextureStorage id = %u - Deleted\n", id);
};