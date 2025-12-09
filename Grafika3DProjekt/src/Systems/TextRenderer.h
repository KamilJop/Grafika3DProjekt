#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <glad/glad.h>
#include "Rendering/Shader.h"

struct Character{
	unsigned int TextureID;
	glm::ivec2   Size;     
	glm::ivec2   Bearing;  
	unsigned int Advance;   
};;

class TextRenderer
{
	public:
	TextRenderer(unsigned int width, unsigned int height);
	~TextRenderer();
	void Load(std::string font, unsigned int fontSize);
	void RenderText(std::string text, float x, float y, float scale, glm::vec4 color);
	float GetTextWidth(std::string text);

	private:
		unsigned int VAO, VBO;
		std::map<char, Character> Characters;
		Shader* textShader;
};

