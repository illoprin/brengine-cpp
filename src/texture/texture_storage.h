#pragma once

#include <util/deps.h>
#include <texture/texture.h>
#include <core/log.h>

namespace b_Texture
{
	// TEXTURE_2D_ARRAY service
	class TextureStorage3D : public Texture
	{
	private:

	public:
		GLsizei layers = 1;
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
			unsigned  layer_index,
			unsigned  offsetX,
			unsigned  offsetY,
			GLuint    format,
			GLuint    dataType,
			void*     pixels
		);

		void bind() override;

		void setWrapping(GLint type) override;
		void setFiltering(GLint type) override;

	};
};