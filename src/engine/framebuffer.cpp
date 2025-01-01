#include "framebuffer.h"

Framebuffer::Framebuffer(Log* logger, const char* name)
{
	this->log = logger;
	this->name = {name};

	glGenFramebuffers(1, &this->id);
	this->bind();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		this->log->logf("[INFO] Framebuffer.%s - id = %u created succesfully\n",
			name, this->id);
	};
	this->unbind();
};

void Framebuffer::initColorAttachment(unsigned w, unsigned h)
{
	this->bind();
	this->color_attachment = new TextureImage2D{this->log, false};
	this->color_attachment->width = w;
	this->color_attachment->height = h;
	this->color_attachment->components = 3;
	this->color_attachment->setFiltering(GL_LINEAR);
	this->color_attachment->setImagePointer(GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// Mipmap level is 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color_attachment->getID(), 0);
	
	this->log->logf("[INFO] Framebuffer.%s - id = %u color attachment created\n",
		this->name.c_str(), this->id);
	this->unbind();
};
void Framebuffer::initDepthAttachment()
{
	this->bind();
	// TODO: Create depth texture and attach it to framebuffer attachment
	this->unbind();
};
void Framebuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);
};
void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};
void Framebuffer::clear(glm::vec4 c)
{
	this->bind();
	glClearColor(c.r, c.g, c.b, c.a);
	if (color_attachment != nullptr && depth_attachment == nullptr)
		glClear(GL_COLOR_BUFFER_BIT);
	else if (color_attachment != nullptr && depth_attachment != nullptr)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
};


GLuint Framebuffer::getID()
{
	return this->id;
};

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &this->id);
	if (this->color_attachment != nullptr)
	{
		this->log->logf("[INFO] Framebuffer.%s - id = %u color attachment texture id = %u deleted\n", 
			this->name.c_str(), this->id, this->color_attachment->getID());
		delete this->color_attachment;
	};

	this->log->logf("[INFO] Framebuffer.%s - id = %u deleted\n",
		this->name.c_str(), this->id);
};