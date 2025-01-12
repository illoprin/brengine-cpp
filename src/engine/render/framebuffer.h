#pragma once

#include "../util/deps.h"
#include "../texture/texture.h"


struct Framebuffer
{
public:
	Framebuffer(const char* name);
	~Framebuffer();
	void initColorAttachment(unsigned, unsigned);
	void initDepthAttachment(unsigned, unsigned);
	void bind();
	void unbind();
	void clear(glm::vec4);
	void check();
	void changeSize(unsigned w, unsigned h);
	// -- Getters
	GLuint getID();
	b_Texture::Texture* getColorAttachment();
	b_Texture::Texture* getDepthAttachment();
	std::string getName();
private:
	GLuint id;
	std::string name{"fb_custom"};

	b_Texture::Texture* color_attachment = nullptr;
	b_Texture::Texture* depth_attachment = nullptr;
};

