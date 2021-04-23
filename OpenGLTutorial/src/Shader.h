#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

class Shader
{
public:
	Shader(const char* VertexPath, const char* FragmentPath)
	{
		std::ifstream vShaderFile, fShaderFile;
		std::string vertexCode, fragmentCode;

		vShaderFile.exceptions(std::ifstream::badbit || std::ifstream::failbit);
		fShaderFile.exceptions(std::ifstream::badbit || std::ifstream::failbit);
		try
		{
			vShaderFile.open(VertexPath);
			fShaderFile.open(FragmentPath);

			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertexShader, fragmentShader;

		// Vertex shader compilation
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, "VERTEX");

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader, "FRAGMENT");

		// Creating shader program and linking shaders
		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	
	void inline Use() { glUseProgram(ID); }

	void inline SetBool (const char* name, bool value) const	{ glUniform1i(glGetUniformLocation(ID, name), (int)value); }
	void inline SetInt  (const char* name, int value) const { glUniform1i(glGetUniformLocation(ID, name), value); }
	void inline SetFloat(const char* name, float value) const { glUniform1f(glGetUniformLocation(ID, name), value); }
	// ------------------------------------------------------------------------------------------------
	void inline setVec2 (const char* name, const glm::vec2& value) const	{ glUniform2fv(glGetUniformLocation(ID, name), 1, &value[0]); }
	void inline setVec2 (const char* name, float x, float y) const { glUniform2f(glGetUniformLocation(ID, name), x, y); }
	// ---------------- -----------------------------------------------------------------------------------
	void inline setVec3 (const char* name, const glm::vec3& value) const { glUniform3fv(glGetUniformLocation(ID, name), 1, &value[0]); }
	void inline setVec3 (const char* name, float x, float y, float z) const { glUniform3f(glGetUniformLocation(ID, name), x, y, z); }
	// ---------------- -----------------------------------------------------------------------------------------
	void inline setVec4 (const char* name, const glm::vec4& value) const { glUniform4fv(glGetUniformLocation(ID, name), 1, &value[0]); }
	void inline setVec4 (const char* name, float x, float y, float z, float w) const { glUniform4f(glGetUniformLocation(ID, name), x, y, z, w); }
	// ---------------- ----------------------------------------------------------------------------------------------
	void inline setMat2 (const char* name, const glm::mat2& mat) const { glUniformMatrix2fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]); }
	// ---------------- ----------------------------------------------------------------------------------------------------
	void inline setMat3 (const char* name, const glm::mat3& mat) const { glUniformMatrix3fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]); }
	// ---------------- ---------------------------------------------------------------------------------------------------------
	void inline setMat4 (const char* name, const glm::mat4& mat) const { glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, &mat[0][0]); }

private:
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}

public:
	unsigned int ID;
};

#endif // SHADER_H
