#pragma once

#include "../utils/deps.h"
#include "texture.h"
#include "../log.h"


struct Framebuffer
{
public:
	Framebuffer(const char* name);
	~Framebuffer();
	void initColorAttachment(unsigned, unsigned);
	void initDepthAttachment(unsigned, unsigned);
	void bind();
	void clear(glm::vec4);
	void check();
	void changeSize(unsigned w, unsigned h);
	// -- Getters
	GLuint getID();
	b_Texture::TextureImage2D* getColorAttachment();
	b_Texture::TextureImage2D* getDepthAttachment();
	std::string getName();
private:
	GLuint id;
	std::string name{"fb_custom"};
	void unbind();

	b_Texture::TextureImage2D* color_attachment = nullptr;
	b_Texture::TextureImage2D* depth_attachment = nullptr;
};
