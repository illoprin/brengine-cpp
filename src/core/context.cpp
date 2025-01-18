#include "context.h"

#include "engine.h"


void b_Context::Init()
{
	glfwMakeContextCurrent(b_Engine::getWindow()); // Now OpenGL draws stuff in this window

	if (glewInit() != GLEW_OK)
	{
		LOG_ERR("Could not init GLEW\n");
		exit(EXIT_FAILURE);
	};

	glewExperimental = true;

	LOG_MSG("Context initialized with render mode %d %d",
		b_Engine::getVidMode().x, b_Engine::getVidMode().y);
};
void b_Context::Prepare()
{
	// glEnable(GL_MULTISAMPLE); // MSAA is not working, because app use multiple framebuffers
	glEnable(GL_BLEND); // Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set alpha blending model
};
void b_Context::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void b_Context::Clear()
{
	b_Context::Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.11f, 0.11f, 0.11f, 1.f);
};
void b_Context::EnableFaceCulling()
{
	glEnable(GL_CULL_FACE); // Enable backface culling
	glCullFace(GL_BACK); // Set backface culling model - vertices order is counter clockwise
	glFrontFace(GL_CCW); // Face culling - front face is in clockwise order
};
void b_Context::DisableFaceCulling()
{
	glDisable(GL_CULL_FACE);
};
void b_Context::EnableDepthTest()
{
	glEnable(GL_DEPTH_TEST);
};
void b_Context::DisableDepthTest()
{
	glDisable(GL_DEPTH_TEST);
};

GLenum b_Context::CheckError(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
		LOG_WAR("%s | %s line %d", error.c_str(), file, line);
    }
    return errorCode;
}
