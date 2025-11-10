#pragma once
#include <glad/glad.h>
#include "stb_image.h"


class Texture
{
public: 
	Texture(const char* fileLoc);
	~Texture();

	bool LoadTexture();
	bool LoadTextureAlpha();
	void UseTexture();
	void ClearTexture();

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;;
	const char* fileLocation;

};

