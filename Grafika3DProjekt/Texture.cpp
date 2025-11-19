#include "Texture.h"


// Constructor
Texture::Texture(const char* fileLoc)
{
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation = fileLoc;
	printf("Texture created: %s\n", fileLocation.c_str());
}

// Clear texture data
void Texture::ClearTexture()
{
	glDeleteTextures(1, &textureID);
	textureID = 0;
	width = 0;
	height = 0;
	bitDepth = 0;
	fileLocation.clear();
}

// Destructor
Texture::~Texture()
{
	ClearTexture();
}

// Load texture from file
bool Texture::LoadTexture()
{
	return LoadTextureAlpha();
}
bool Texture::LoadTextureAlpha()
{
	printf("Loading texture with alpha: %s\n", fileLocation.c_str());
	fflush(stdout);
	unsigned char* texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to load texture: %s\n", fileLocation.c_str());
		fflush(stdout);
		return false;
	}
	GLenum internalFormat = 0;
	GLenum dataFormat = 0;

	if (bitDepth == 4) {
		internalFormat = GL_RGBA;
		dataFormat = GL_RGBA;
	}
	else if (bitDepth == 3) {
		internalFormat = GL_RGB;
		dataFormat = GL_RGB;
	}
	else {
		printf("Texture format not supported (BitDepth: %d): %s\n", bitDepth, fileLocation.c_str());
		stbi_image_free(texData);
		return false;
	}
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, texData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

bool Texture::LoadNormalMap()
{
	printf("Loading normal map texture: %s\n", fileLocation.c_str());
	fflush(stdout);
	unsigned char* texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
	if (!texData)
	{
		printf("Failed to load texture: %s\n", fileLocation.c_str());
		fflush(stdout);
		return false;
	}
	GLenum internalFormat = GL_RGB8;
	GLenum dataFormat = 0;

	if (bitDepth == 3)      
		dataFormat = GL_RGB;
	else if (bitDepth == 4) 
		dataFormat = GL_RGBA;
	else {
		printf("Unsupported normal map channel count: %d\n", bitDepth);
		stbi_image_free(texData);
		return false;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, texData);

	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(texData);

	return true;
}

// Use the texture
void Texture::UseTexture(GLenum textureUnit)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}