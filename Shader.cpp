#include <stdexcept>

#include "Shader.hpp"

Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
    //Get vertex shader content

    std::ifstream vertexShaderFilestream(vertexPath);
	if (!vertexShaderFilestream.is_open())
	{
		throw std::runtime_error("Failed to read vertex shader.");
	}
	std::stringstream vertexShaderStringstream;
	vertexShaderStringstream << vertexShaderFilestream.rdbuf();
	std::string vertexShaderString = vertexShaderStringstream.str();
	const char * vertexShaderChar = vertexShaderString.c_str();

    //Compile vertex shader content

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderChar, NULL);
	glCompileShader(vertexShader);
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		throw std::runtime_error("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" + std::string(infoLog));
	}

    //Get fragment shader content

	std::ifstream fragmentShaderFilestream(fragmentPath);
	if (!fragmentShaderFilestream.is_open())
	{
		throw std::runtime_error("Failed to read fragment shader.");
	}
	std::stringstream fragmentShaderStringstream;
	fragmentShaderStringstream << fragmentShaderFilestream.rdbuf();
	std::string fragmentShaderString = fragmentShaderStringstream.str();
	const char * fragmentShaderChar = fragmentShaderString.c_str();

    //Compile fragment shader content

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderChar, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		throw std::runtime_error("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" + std::string(infoLog));
	}

    //Link shaders into a program

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		throw std::runtime_error("ERROR::SHADER::PROGRAM::LINKING_FAILED\n" + std::string(infoLog));
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::use()
{
    glUseProgram(program);
}
