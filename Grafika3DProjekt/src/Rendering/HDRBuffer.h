#pragma once
#include <iostream>
#include <glad/glad.h>
class HDRBuffer
{
public:
	HDRBuffer(int width, int height);
	~HDRBuffer();

	void Bind();
	void Unbind();

	unsigned int getColorBuffer() { return colorBuffer; }

	void RenderQuad();



private:
	unsigned int FBO;
	unsigned int colorBuffer;
	unsigned int RBO;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	void InitQuad();
};

