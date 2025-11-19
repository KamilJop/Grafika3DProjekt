#pragma once
#include <glad/glad.h>
#include "stb_loader.h"
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

private:
	GLuint textureID;
	int width;
	int height;
	int bitDepth;;
	std::string fileLocation;

};

