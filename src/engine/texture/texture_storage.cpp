#include "texture_storage.h"

#include "../core/engine.h"

using namespace b_Texture;

TextureStorage3D::TextureStorage3D()
{
	b_log->logf("[INFO] TextureStorage id = %u - Generated\n", id);
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

TextureStorage3D::~TextureStorage3D()
{
	b_log->logf("[INFO] TextureStorage id = %u - Deleted\n", id);
};