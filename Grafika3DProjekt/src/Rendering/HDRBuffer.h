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

	unsigned int getColorBuffer() { return colorBuffers[0]; }
	unsigned int getBrightTexture() { return colorBuffers[1]; }

	unsigned int getPingPongFBO(int index) { return pingPongFBO[index]; }
	unsigned int getPingPongColorBuffer(int index) { return pingPongColorbuffers[index]; }

	void RenderQuad();



private:
	unsigned int FBO;
	unsigned int colorBuffers[2];

	unsigned int pingPongFBO[2];
	unsigned int pingPongColorbuffers[2];

	unsigned int RBO;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	void InitQuad();
};

