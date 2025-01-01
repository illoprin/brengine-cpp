#pragma once

#include "utils/deps.h"
#include "log.h"
#include "utils/utils.h"

class Program
{
public:
	Program(Log*);
	~Program();

	void InitVertexAndFragment(std::string progname);

	// Delete copy operators
	Program(Program&) = delete;
	Program& operator=(Program&) = delete;
	Program& operator=(const Program&) = delete;

	static GLuint CompileShader(Log*, const char* source, GLuint type);
	void LinkProgram(size_t count, ...);

	static inline GLuint GetUniformID(Log*, GLuint programID, const char* name);

	// Uniform setters
	void set1f(float, const char* name);
	void set1i(int, const char* name);
	void set2f(glm::vec2, const char* name);
	void set3f(glm::vec3, const char* name);
	void set4f(glm::vec4, const char* name);
	void setmat4(glm::mat4, const char* name);
	void setmat2(glm::mat2, const char* name);

	void use();

	GLuint getID();
	GLuint getShaderID(unsigned);
private:
	Log* log;
	GLuint id;
	std::vector<GLuint> shaders;
};