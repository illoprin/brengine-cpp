#include "program.h"

Program::Program(Log* logger, std::string progname)
{
	this->log = logger;
	std::string fs_source = "\0";
	std::string vs_source = "\0";

	std::string vs_filepath
		{"assets/shaders/" + progname + ".vert"};
	std::string fs_filepath
		{"assets/shaders/" + progname + ".frag"};

	b_Utils::read_file_lines(vs_filepath.c_str(), vs_source);
	b_Utils::read_file_lines(fs_filepath.c_str(), fs_source);

	try
	{
		GLuint vs_id = Program::CompileShader(this->log, vs_source.c_str(), GL_VERTEX_SHADER);
		GLuint fs_id = Program::CompileShader(this->log, fs_source.c_str(), GL_FRAGMENT_SHADER);
		this->shaders.push_back(vs_id);
		this->shaders.push_back(fs_id);
		this->id = Program::CreateAndLinkProgram(this->log, 2, vs_id, fs_id);
	}
	catch (...)
	{
		fprintf(stderr, "Exception: Program initialization error!\n");
	}
};

Program::~Program()
{
	for (GLuint s_id : this->shaders)
	{
		glDetachShader(this->id, s_id);
		glDeleteShader(s_id);
		this->log->logf("[INFO] Shader id = %u released\n", s_id);
	};

	glDeleteProgram(this->id);

	this->log->logf("[INFO] Program id = %u released\n", this->id);
};

GLuint Program::CompileShader(Log* log, const char* source, GLuint type)
{
	GLuint id = glCreateShader(type);
	glShaderSource(id, 1, &source, NULL);
	glCompileShader(id);
	int success; char compile_log[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(id, 512, NULL, compile_log);
		log->logf("[WARNING] Program - Shader with type %X compiled with errors\nError log: %s\n", type, log);
		throw "Shader compilation error";
	}
	log->logf("[INFO] Program - shader id = %u type = %u compiled successfully!\n", id, type);
	return id;
};
GLuint Program::CreateAndLinkProgram(Log* log, size_t count, ...)
{
	GLuint prog_id = glCreateProgram();

	va_list shaders;
	va_start(shaders, log);
	for (size_t i = 0; i < count; i++)
	{
		GLuint s_id = va_arg(shaders, GLuint);
		glAttachShader(prog_id, s_id);
	};

	glLinkProgram(prog_id);

	int success; char info_log[512];
	glGetProgramiv(prog_id, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(prog_id, 512, NULL, info_log);
		log->logf("[WARNING] Program id = %u linked with errors\nError log: %s", prog_id, info_log);
		throw "Program link error!";
	}
	log->logf("[INFO] Program id = %u linked successfully\n", prog_id);
	return prog_id;
};

GLuint Program::GetUniformID(Log* log, GLuint programID, const char* name)
{
	GLuint id = glGetUniformLocation(programID, name);
	if (id < 0)
	{
		log->logf("[WARNING] Program %u: Non-existent uniform with name %s\n", programID, name);
		return -1;
	};
	
	return id;
};

// Uniform setters
void Program::set1f(float value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->log, this->id, name);
	if (uid >= 0) glUniform1f(uid, value);
};
void Program::set1i(int value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->log, this->id, name);
	if (uid >= 0) glUniform1i(uid, value);
};
void Program::set2f(glm::vec2 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->log, this->id, name);
	if (uid >= 0) glUniform2fv(uid, 1, glm::value_ptr(value));
};
void Program::set3f(glm::vec3 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->log, this->id, name);
	if (uid >= 0) glUniform3fv(uid, 1, glm::value_ptr(value));
};
void Program::setmat4(glm::mat4 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->log, this->id, name);
	if (uid >= 0) glUniformMatrix4fv(uid, 1, GL_FALSE, glm::value_ptr(value));
};
void Program::setmat2(glm::mat2 value, const char* name)
{
	GLuint uid = Program::GetUniformID(this->log, this->id, name);
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
GLuint Program::getFragmentID()
{
	return this->shaders[0];
};
GLuint Program::getVertexID()
{
	return this->shaders[1];
};