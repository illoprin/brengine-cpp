#include "texture_model.h"



using namespace b_Texture;

TextureModel::TextureModel(bool mp)
{
	use_mipmapping = mp;
};

void TextureModel::FromPNG(std::string fn)
{
	std::string file_path{TEXTURE_FILE_PATH(fn)};

	int _width, _height, _channels;

	stbi_set_flip_vertically_on_load(1);

	unsigned char* bytes = stbi_load(
		file_path.c_str(), 
		&_width,
		&_height,
		&_channels, 0
	);

	stbi_set_flip_vertically_on_load(0);

	if (bytes != NULL)
	{
		this->width = _width;
		this->height = _height;
		this->components = _channels;

		if (this->use_mipmapping)
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

		LOG_MSG("Texture id = %u loaded from file with path %s", id, file_path.c_str());
	}
	else
	{
		LOG_WAR("Could not open image with path %s", file_path.c_str());
	}
};

void TextureModel::FromBMP(std::string fn)
{
	std::string file_path{TEXTURE_FILE_PATH(fn)};

	int _width, _height, _channels;

	stbi_set_flip_vertically_on_load(1);

	unsigned char* bytes = stbi_load(
		file_path.c_str(), 
		&_width,
		&_height,
		&_channels, 0
	);

	stbi_set_flip_vertically_on_load(0);

	if (bytes != NULL)
	{
		this->width = _width;
		this->height = _height;
		this->components = _channels;

		if (this->use_mipmapping)
			this->setFilteringMipmap(GL_NEAREST, GL_LINEAR_MIPMAP_LINEAR);
		else
			this->setFiltering(GL_NEAREST);
		
		this->setWrapping(GL_REPEAT);

		this->setImagePointer(
			GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, bytes
		);
		if (this->use_mipmapping)
			glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(bytes);

		LOG_MSG("Texture id = %u loaded from file with path %s", id, file_path.c_str());
	}
	else
	{
		LOG_WAR("Could not open image with path %s", file_path.c_str());
	}
};

void TextureModel::setFilteringMipmap(GLint type_min, GLint type_mag)
{
	this->bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, type_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, type_mag);
};