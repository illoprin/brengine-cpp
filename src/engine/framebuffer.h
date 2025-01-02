#pragma once

#include "utils/deps.h"
#include "texture.h"
#include "log.h"

struct Framebuffer
{
public:
	Framebuffer(Log* logger, const char* name);
	~Framebuffer();
	void initColorAttachment(unsigned, unsigned);
	void initDepthAttachment(unsigned, unsigned);
	void bind();
	void clear(glm::vec4);
	void check();
	// -- Getters
	GLuint getID();
	TextureImage2D* getColorAttachment();
	TextureImage2D* getDepthAttachment();
	std::string getName();
private:
	GLuint id;
	std::string name{"fb_custom"};
	void unbind();

	TextureImage2D* color_attachment = nullptr;
	TextureImage2D* depth_attachment = nullptr;
	
	Log* log;
};
