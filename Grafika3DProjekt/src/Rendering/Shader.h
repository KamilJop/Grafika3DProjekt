#pragma once

#include "stdio.h"
#include <string>
#include <iostream>
#include <fstream>

#include <glm\glm.hpp>
#include <glad\glad.h>
#include <glm/gtc/type_ptr.hpp>


class Shader
{
public:
	Shader();

	void CreateShader(const char* vertexPath, const char* fragmentPath);
	void CreateShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);

	std::string ReadFile(const char* filePath);

	void UseShader();
	void ClearShader();
	int getShaderID() const { return shaderId; }


	void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(shaderId, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}

	void setVec3(const std::string& name, const glm::vec3& value) const {
		glUniform3fv(glGetUniformLocation(shaderId, name.c_str()), 1, glm::value_ptr(value));
	}

	void setVec4(const std::string& name, const glm::vec4& value) const {
		glUniform4fv(glGetUniformLocation(shaderId, name.c_str()), 1, glm::value_ptr(value));
	}

	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(shaderId, name.c_str()), value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(shaderId, name.c_str()), value);
	}



	~Shader();

private:

	GLuint shaderId;


	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void CompileShader(const char* vertexCode, const char* geometryCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

};


