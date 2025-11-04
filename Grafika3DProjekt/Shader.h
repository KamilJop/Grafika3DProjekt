#pragma once

#include "stdio.h"
#include <string>
#include <iostream>
#include <fstream>

#include <glad\glad.h>


class Shader
{
public:
	Shader();

	void CreateShader(const char* vertexPath, const char* fragmentPath);

	std::string ReadFile(const char* filePath);

	void UseShader();
	void ClearShader();

	GLuint getProjectionUniformLocation();
	GLuint getModelUniformLocation();
	GLuint getViewMatrixUniformLocation();

	~Shader();

private:

	GLuint shaderId;
	GLuint uniformProjection;
	GLuint uniformModel;
	GLuint uniformViewMatrix;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);

};


