#include "texture.h"

#include "../core/engine.h"
#include "../core/context.h"

using namespace b_Texture;

Texture::Texture()
{
	glGenTextures(1, &this->id);
	b_log->logf("[INFO] Texture id = %u - Generated new texture object\n", id);
};

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, this->id);
};

void Texture::setWrapping(GLint type)
{
	this->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, type);
};

void Texture::setFiltering(GLint type)
{
	this->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);
};

void Texture::setImagePointer(
	GLint  internalFormat, 
	GLint  format, 
	GLint  dataType, 
	void*  data
)
{
	this->bind();
	// Allocate vidmemory for texture
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
		this->width, this->height, 0,
		format, dataType, data
	);
	b_CheckError(); // Checking OpenGL errors

	b_log->logf("[INFO] Texture id = %u data pointer created\n", this->id);
};


GLuint Texture::getID()
{
	return this->id;
};

Texture::~Texture()
{
	b_log->logf("[INFO] Texture id = %u released\n", this->id);
	glDeleteTextures(1, &this->id);
};

void b_Texture::bindToSlot(unsigned slot, b_Texture::Texture* tex)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	tex->bind();
};