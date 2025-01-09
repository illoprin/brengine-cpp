#include "framebuffer.h"

#include "../engine.h"

Framebuffer::Framebuffer(const char* name)
{
	this->name = {name};

	glGenFramebuffers(1, &this->id);
};

void Framebuffer::changeSize(unsigned w, unsigned h)
{
	if (this->color_attachment != nullptr)
	{
		this->color_attachment->width = w;
		this->color_attachment->height = h;
		this->color_attachment->setImagePointer(
			GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, NULL
		);
	};

	if (this->depth_attachment != nullptr)
	{
		this->depth_attachment->width = w;
		this->depth_attachment->height = h;
		this->depth_attachment->setImagePointer(
			GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
		);
	};
};

void Framebuffer::initColorAttachment(unsigned w, unsigned h)
{
	this->bind();
	this->color_attachment = new TextureImage2D{false};
	this->color_attachment->width = w;
	this->color_attachment->height = h;
	this->color_attachment->components = 4;
	this->color_attachment->setFiltering(GL_LINEAR);
	this->color_attachment->setImagePointer(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	// Mipmap level is 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color_attachment->getID(), 0);
	
	b_log->logf("[INFO] Framebuffer.%s id = %u - color attachment created\n",
		this->name.c_str(), this->id);
	this->unbind();
};
void Framebuffer::initDepthAttachment(unsigned w, unsigned h)
{
	this->bind();
	this->depth_attachment = new TextureImage2D(false);
	this->depth_attachment->width = w;
	this->depth_attachment->height = h;
	this->depth_attachment->components = 1;
	this->depth_attachment->setImagePointer(
		GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
	);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
		this->depth_attachment->getID(), 0
	);

	b_log->logf("[INFO] Framebuffer.%s id = %u - depth attachment created\n",
		this->name.c_str(), this->id);
	this->unbind();
};

void Framebuffer::check()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		b_log->logf("[INFO] Framebuffer.%s id = %u - attachments completed!\n",
			this->name.c_str(), this->id);
	}
	else
	{
		b_log->logf("[WARNING] Framebuffer.%s id = %u - attachments binding not completed\n",
			this->name.c_str(), this->id);
	};
}

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

	if (color_attachment != nullptr
		&& depth_attachment == nullptr)
		glClear(GL_COLOR_BUFFER_BIT);
	else if (color_attachment != nullptr
			&& depth_attachment != nullptr)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(c.r, c.g, c.b, c.a);

};

// --- Getters
GLuint Framebuffer::getID()
{
	return this->id;
};
std::string Framebuffer::getName()
{
	return this->name;
};
TextureImage2D* Framebuffer::getColorAttachment()
{
	return this->color_attachment;
};
TextureImage2D* Framebuffer::getDepthAttachment()
{
	return this->depth_attachment;
};


Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &this->id);
	if (this->color_attachment != nullptr)
	{
		b_log->logf("[INFO] Framebuffer.%s id = %u - color attachment id = %u deleted\n", 
			this->name.c_str(), this->id, this->color_attachment->getID());
		delete this->color_attachment;
	};
	if (this->depth_attachment != nullptr)
	{
		b_log->logf("[INFO] Framebuffer.%s id = %u - depth attachment id = %u deleted\n", 
			this->name.c_str(), this->id, this->depth_attachment->getID());
		delete this->depth_attachment;
	};

	b_log->logf("[INFO] Framebuffer.%s id = %u - deleted\n",
		this->name.c_str(), this->id);
};