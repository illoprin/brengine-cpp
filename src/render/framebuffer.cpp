#include "framebuffer.h"



using namespace b_Texture;

Framebuffer::Framebuffer(const char* name)
{
	this->name = {name};

	glGenFramebuffers(1, &this->id);

	LOG_MSG("Framebuffer.%s created", name);
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
	this->color_attachment = new Texture{};
	this->color_attachment->width = w;
	this->color_attachment->height = h;
	this->color_attachment->components = 4;
	this->color_attachment->setFiltering(GL_LINEAR);
	this->color_attachment->setImagePointer(GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	// Mipmap level is 0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->color_attachment->getID(), 0);
	b_CheckError(); // Checking OpenGL errors
	
	LOG_MSG("Framebuffer.%s color attachment created X: %u Y: %u", name.c_str(), w, h);
	this->unbind();
};
void Framebuffer::initDepthAttachment(unsigned w, unsigned h)
{
	this->bind();
	this->depth_attachment = new Texture{};
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
	b_CheckError(); // Checking OpenGL errors

	LOG_MSG("Framebuffer.%s depth attachment created X: %u Y: %u", name.c_str(), w, h);
	this->unbind();
};

void Framebuffer::check()
{
	this->bind();
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		LOG_MSG("Framebuffer.%s attachments completed", name.c_str());
	}
	else
	{
		LOG_MSG("Framebuffer.%s attachments bindings not completed", name.c_str());
	};
	this->unbind();
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

	this->unbind();
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
Texture* Framebuffer::getColorAttachment()
{
	return this->color_attachment;
};
Texture* Framebuffer::getDepthAttachment()
{
	return this->depth_attachment;
};


Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &this->id);
	if (this->color_attachment != nullptr)
	{
		LOG_MSG("Framebuffer.%s color attachment id = %u deleted", name.c_str(),
			color_attachment->getID());
		delete this->color_attachment;
	};
	if (this->depth_attachment != nullptr)
	{
		LOG_MSG("Framebuffer.%s depth attachment id = %u deleted", name.c_str(),
			depth_attachment->getID());
		delete this->depth_attachment;
	};

	LOG_MSG("Framebuffer.%s released", name.c_str());
};