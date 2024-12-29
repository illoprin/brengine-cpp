#include "texture.h"

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

void TextureImage2D::FromFile(std::string assets_path)
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

		if (this->use_mipmaps)
			this->setFilteringMipmap(GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
		else
			this->setFiltering(GL_NEAREST);
		
		this->setWrapping(GL_REPEAT);

		if (_channels == 3) this->setImagePointer(
			GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, bytes
		);
		else if (_channels == 4) this->setImagePointer(
			GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, bytes
		);

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

void TextureImage2D::FromBytes(
	unsigned width, 
	unsigned height, 
	unsigned channels,
	GLint components,
	std::vector<unsigned char>& bytes
)
{
	if (bytes.size() < channels)
	{
		this->log->logf("[ERROR] Texture - could not init, byte size is less then %u\n", channels);
		return;
	}
	if (width < 1 || height < 1 || channels < 1)
	{
		this->log->logf("[ERROR] Texture - could not init, components value is less than 1\n");
		return;
	}
	
	this->width = width;
	this->height = height;
	this->components = channels;

	if (this->use_mipmaps)
		this->setFilteringMipmap(GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
	else
		this->setFiltering(GL_NEAREST);
		
	this->setWrapping(GL_REPEAT);
	
	
	this->setImagePointer(components, components, GL_UNSIGNED_BYTE, &bytes[0]);
	
	this->log->logf(
		"[INFO] Texture - Inited from bytes, width is %u height is %u\n",
		this->width, this->height
	);
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

inline void TextureImage2D::setFilteringMipmap(GLint type_min, GLint type_mag)
{
	this->bind();
	if (this->use_mipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type_min);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type_mag);
	}
};

void TextureImage2D::setImagePointer(
	GLint internalFormat, 
	GLint format, 
	GLint dataType, 
	unsigned char* bytes
)
{
	this->bind();
	if (!this->inited_with_image)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat,
			this->width, this->height, 0,
			format, dataType, bytes
		);
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