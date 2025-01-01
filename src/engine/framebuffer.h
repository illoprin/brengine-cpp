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
	void initDepthAttachment();
	void bind();
	void clear(glm::vec4);
	void unbind();

	GLuint getID();
private:
	GLuint id;
	std::string name;

	TextureImage2D* color_attachment = nullptr;
	TextureImage2D* depth_attachment = nullptr;
	
	Log* log;
};
