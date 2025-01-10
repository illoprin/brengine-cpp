#pragma once

#include "../utils/deps.h"

namespace b_Texture
{
	// TEXTURE_2D_ARRAY service
	struct TextureStorage3D
	{
	private:

	public:
		/*
			Several textures with one size in one TextureObject
		*/
		TextureStorage3D();
		~TextureStorage3D();

		// Set width, height, layers values of struct before call this function
		void InitStorage(GLuint internalFormat);
		
		/*
			Sends pixels to vidmemory
			You can send it multiple times with one pixel-array with different offsets
			Or you can use different pixel-arrays with zero offset
		*/
		void setImagePointer(
			uint8_t   layer_index,
			uint16_t  offsetX,
			uint16_t  offsetY,
			GLuint    format,
			GLuint    dataType,
			void*     pixels
		);

		void setFiltering(GLuint);
		void setWrapping(GLuint);

		void bind();
		uint16_t width;
		uint16_t height;
		uint8_t layers;
		GLuint id;
	};
};