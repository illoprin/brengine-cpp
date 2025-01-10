#pragma once

#include "../utils/deps.h"
#include "../log.h"

namespace b_Texture
{

	struct TextureImage2D {

	private:
		bool inited_with_image;
		bool use_mipmaps;
		GLuint id;
	public:
		// Gen OpenGL texture object, mp - use MipMapping
		TextureImage2D(bool mp);
		~TextureImage2D();
		
		uint16_t width;
		uint16_t height;
		u_char components;
		/*
			You can use this color (1.0, 0.09, 0.79) #ff17c9 to indicate transperent areas in your image
			0 - texture not use transperency color, 1 - texture use transperency color
		*/
		unsigned short use_transperency_mask = 0;

		// Delete copy constructor
		TextureImage2D& operator=(TextureImage2D&) = delete;
		TextureImage2D(TextureImage2D&) = delete;
		TextureImage2D(const TextureImage2D&) = delete; 

		/*
		*	PNG only
		* 	Path string: $app_dir/assets/textures/$filename.png
		*/
		void FromPNG(std::string filename);
		/*
		* BMP only
		* Path string: $app_dir/assets/textures/$filename.png 
		*/
		void FromBMP(std::string filename);
		void FromBytes(
			unsigned                    width, 
			unsigned                    height,
			unsigned                    channels,
			GLint                       components,
			std::vector<unsigned char>& bytes
		);

		void setWrapping(GLint type);
		void setFiltering(GLint type);
		void setFilteringMipmap(GLint type_min, GLint type_mag);
		/*
		* 	Allocate vidmemory for texture
		*/
		void setImagePointer(GLint internalFormat, GLint format, GLint dataType, void* data);

		void bind();

		GLuint getID();
	};

	void bindToSlot(unsigned, TextureImage2D*);
}