#pragma once
#include <glad/glad.h>
#include "stb_image.h"


class Texture
{
public: 
	Texture(char* fileLoc);
	~Texture();

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;;
	char* fileLocation;

};

