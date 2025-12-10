#pragma once
#include <glad/glad.h>
#include "External/stb_loader.h"
#include <string>


class Texture
{
public: 
	Texture(const char* fileLoc);
	~Texture();

	bool LoadTexture();
	bool LoadTextureAlpha();
	bool LoadMaps();
	void UseTexture(GLenum textureUnit);
	void ClearTexture();
	GLuint getTextureID() { return textureID; }

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;;
	std::string fileLocation;

};

