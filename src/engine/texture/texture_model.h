#pragma once

#include "texture.h"

namespace b_Texture
{
	class TextureModel : public Texture
	{
	public:
		TextureModel(bool use_mipmapping = false);

		bool use_mipmapping = false;
		
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

		/*
			Set filtering with mipmap using
		*/
		void setFilteringMipmap(GLint min, GLint mag);
	};
};