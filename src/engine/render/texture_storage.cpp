#include "texture_storage.h"

#include "../engine.h"

using namespace b_Texture;

TextureStorage3D::TextureStorage3D()
{
	glGenTextures(1, &id);
	b_log->logf("[INFO] TextureStorage id = %u - Generated\n", id);
};

void TextureStorage3D::bind()
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, id);
};

void TextureStorage3D::InitStorage(GLuint internalFormat)
{
	bind();
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalFormat, width, height, (GLsizei)layers);
};

void TextureStorage3D::setImagePointer(
	uint8_t l, uint16_t ox, uint16_t oy, GLuint f, GLuint dt, void* p
)
{
	bind();
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, ox, oy, l, width, height, 1, f, dt, p);
};

void TextureStorage3D::setFiltering(GLuint t)
{
	bind();
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, t);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, t);
};

void TextureStorage3D::setWrapping(GLuint t)
{
	bind();
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, t);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, t);
};

TextureStorage3D::~TextureStorage3D()
{
	glDeleteTextures(1, &id);
	b_log->logf("[INFO] TextureStorage id = %u - Deleted\n", id);

};