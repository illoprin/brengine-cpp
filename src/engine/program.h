#pragma once

#include "utils/deps.h"
#include "log.h"
#include "utils/utils.h"

class Program
{
public:
	Program(Log* logger, std::string progname);
	~Program();

	// Delete copy operators
	Program(Program&) = delete;
	Program& operator=(Program&) = delete;
	Program& operator=(const Program&) = delete;

	static GLuint CompileShader(Log* log, const char* source, GLuint type);
	static GLuint CreateAndLinkProgram(Log* log, size_t count, ...);

	static inline GLuint GetUniformID(Log* log, GLuint programID, const char* name);

	// Uniform setters
	void set1f(float value, const char* name);
	void set1i(int value, const char* name);
	void set2f(glm::vec2 value, const char* name);
	void set3f(glm::vec3 value, const char* name);
	void setmat4(glm::mat4 value, const char* name);
	void setmat2(glm::mat2 value, const char* name);

	void use();

	GLuint getID();
	GLuint getFragmentID();
	GLuint getVertexID();
private:
	Log* log;
	GLuint id;
	std::vector<GLuint> shaders;
};