#include "program.h"

Program::Program()
{
	this->id = glCreateProgram();
	LOG_MSG("Program id = %u created", id);
};

void Program::InitVertexAndFragment(std::string progname)
{
	std::string fs_source = "\0";
	std::string vs_source = "\0";

	std::string vs_filepath{SHADER_FILE_PATH(progname) + ".vert"};
	std::string fs_filepath{SHADER_FILE_PATH(progname) + ".frag"};

	b_Utils::ReadFileLines(vs_filepath.c_str(), vs_source);
	b_Utils::ReadFileLines(fs_filepath.c_str(), fs_source);
	
	if (vs_source[0] != '\0' && fs_source[0] != '\0')
	{
		try
		{
			GLuint vs_id = Program::CompileShader(vs_source.c_str(), GL_VERTEX_SHADER);
			GLuint fs_id = Program::CompileShader(fs_source.c_str(), GL_FRAGMENT_SHADER);
			this->shaders.push_back(vs_id);
			this->shaders.push_back(fs_id);
			this->LinkProgram(2, vs_id, fs_id);
		}
		catch (...)
		{
			LOG_ERR("Link error");
		}
	}
	else
	{
		LOG_WAR("Shader source file is empty");
	}
}

Program::~Program()
{
	for (GLuint s_id : this->shaders)
	{
		glDetachShader(this->id, s_id);
		glDeleteShader(s_id);
		LOG_MSG("Program shader id = %u released", s_id);
	};

	glDeleteProgram(this->id);

	LOG_MSG("Program id = %u released", id);
};

GLuint Program::CompileShader(const char* source, GLuint type)
{
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);

	GLint success; char compile_log[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE)
	{
		GLint log_length = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);

		glGetShaderInfoLog(id, log_length, &log_length, &compile_log[0]);
		LOG_ERR("Shader type 0x%X compilation error\nError log: %s", type, compile_log);
		glDeleteShader(id);
		throw "Shader compilation error";
	}

	LOG_MSG("Shader id = %u type = 0x%X compiled", id, type);
	return id;
};
void Program::LinkProgram(size_t count, ...)
{
	va_list args;
	va_start(args, count);
	for (size_t i = 0; i < count; i++)
		glAttachShader(this->id, va_arg(args, GLuint));

	glLinkProgram(this->id);

	GLint success; char info_log[512];
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		glGetProgramInfoLog(this->id, 512, NULL, info_log);
		LOG_ERR("Program id = %u link error\nError log: %s", this->id, info_log);
		throw "Program link error!";
	}
	LOG_MSG("Program id = %u linked successfully", this->id);
};

GLuint Program::GetUniformID(GLuint programID, const char* name)
{
	GLuint id = glGetUniformLocation(programID, name);
	if (id < 0)
		LOG_WAR("Program id = %u undefined uniform name %s", programID, name);
	return id;
};

// Uniform setters
void Program::set1f(float value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->id, name);
	if (uid >= 0) glUniform1f(uid, value);
};
void Program::set1i(int value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->id, name);
	if (uid >= 0) glUniform1i(uid, value);
};
void Program::set2f(glm::vec2 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->id, name);
	if (uid >= 0) glUniform2fv(uid, 1, glm::value_ptr(value));
};
void Program::set3f(glm::vec3 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->id, name);
	if (uid >= 0) glUniform3fv(uid, 1, glm::value_ptr(value));
};
void Program::set4f(glm::vec4 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->id, name);
	if (uid >= 0) glUniform4fv(uid, 1, glm::value_ptr(value));
};

void Program::setmat4(glm::mat4 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->id, name);
	if (uid >= 0) glUniformMatrix4fv(uid, 1, GL_FALSE, glm::value_ptr(value));
};
void Program::setmat2(glm::mat2 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->id, name);
	if (uid >= 0) glUniformMatrix2fv(uid, 1, GL_FALSE, glm::value_ptr(value));
};

void Program::use()
{
	glUseProgram(this->id);
};

GLuint Program::getID()
{
	return this->id;
};
GLuint Program::getShaderID(unsigned id)
{
	return this->shaders[id];
};