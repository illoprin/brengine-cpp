#pragma once

#include <core/context.h>
#include <util/deps.h>
#include <core/log.h>

namespace b_Texture
{

	class Texture {

	protected:
		GLuint id;
	public:
		Texture();
		~Texture();
		
		GLsizei width = 64;
		GLsizei height = 64;
		GLsizei components = 3;

		/*
			You can use this color (1.0, 0.09, 0.79) #ff17c9 to indicate transperent areas in your image
			0 - texture not use transperency color, 1 - texture use transperency color
		*/
		unsigned short use_transperency_mask = 0;
		
		// Delete copy operators
		Texture& operator=(Texture&) = delete;
		Texture(Texture&) = delete;
		Texture(const Texture&) = delete; 

		virtual void setWrapping(GLint type);
		virtual void setFiltering(GLint type);

		/*
		* 	Allocate vidmemory for texture
		*/
		virtual void setImagePointer(GLint internalFormat, GLint format, GLint dataType, void* data);

		virtual void bind();

		GLuint getID();
	};

	void bindToSlot(unsigned, Texture*);
}