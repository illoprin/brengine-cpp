#include "texture_storage.h"

#include "../core/engine.h"
#include "../core/context.h"

using namespace b_Texture;

TextureStorage3D::TextureStorage3D()
{
	b_log->logf("[INFO] TextureStorage id = %u - Generated\n", id);
};

void TextureStorage3D::InitStorage(GLuint internalFormat)
{
	this->bind();
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internalFormat, width, height, layers);
	b_log->logf("[INFO] TextureStorage id = %u - Inited with width %u height %u layer count is %u\n", id, width, height, layers);
	b_CheckError(); // Checking OpenGL errors
};

void TextureStorage3D::setImagePointer(
	uint8_t l, uint16_t ox, uint16_t oy, GLuint f, GLuint dt, void* p
)
{
	this->bind();
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, ox, oy, l, width, height, layers, f, dt, p);
	b_CheckError(); // Checking OpenGL errors
};

TextureStorage3D::~TextureStorage3D()
{
	b_log->logf("[INFO] TextureStorage id = %u - Deleted\n", id);
};