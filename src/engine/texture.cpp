#include "texture.h"

// STB image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

TextureImage2D::TextureImage2D(Log* logger, bool mp = false)
{
	this->log = logger;
	glGenTextures(1, &this->id);

	this->inited_with_image = false;
	this->use_mipmaps = mp;
};

void TextureImage2D::bind()
{
	glBindTexture(GL_TEXTURE_2D, this->id);
};

void TextureImage2D::initFromFile(std::string assets_path)
{
	std::string file_path{"assets/" + assets_path + ".png"};

	int _width, _height, _channels;

	stbi_set_flip_vertically_on_load(1);

	unsigned char* bytes = stbi_load(
		file_path.c_str(), 
		&_width,
		&_height,
		&_channels, 0
	);

	if (bytes != NULL)
	{
		this->width = _width;
		this->height = _height;
		this->components = _channels;

		this->bind();

		if (this->use_mipmaps)
			this->setFilteringMipmap(GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
		else
			this->setFiltering(GL_NEAREST);
		
		this->setWrapping(GL_REPEAT);

		if (_channels == 3) this->sendBytes(GL_RGB, bytes);
		else if (_channels == 4) this->sendBytes(GL_RGBA, bytes);

		stbi_image_free(bytes);

		this->inited_with_image = true;
		this->log->logf("[INFO] Texture id = %u loaded from file with path %s\n",
			this->id, file_path.c_str()
		);
	}
	else
	{
		this->log->logf("[ERROR] Texture: Could not open image with path %s\n",
			file_path.c_str()
		);
	}
};

inline void TextureImage2D::setWrapping(GLint type)
{
	this->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, type);
};

inline void TextureImage2D::setFiltering(GLint type)
{
	this->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type);
};

void TextureImage2D::setFilteringMipmap(GLint type_min, GLint type_mag)
{
	this->bind();
	if (this->use_mipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type_min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type_mag);
	}
};

void TextureImage2D::sendBytes(GLint comps, unsigned char* bytes)
{
	this->bind();
	if (!this->inited_with_image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, comps, this->width, this->height, 0, comps, GL_UNSIGNED_BYTE, bytes);
		if (this->use_mipmaps)	glGenerateMipmap(GL_TEXTURE_2D);
	}
};

TextureImage2D::~TextureImage2D()
{
	this->log->logf("[INFO] Texture id = %u released\n", this->id);
	glDeleteTextures(1, &this->id);
};

inline void useTextureInSlot(GLint slot, TextureImage2D& tex)
{
	glActiveTexture(slot);
	tex.bind();
};