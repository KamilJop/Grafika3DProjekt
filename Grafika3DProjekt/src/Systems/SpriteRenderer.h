#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"





class SpriteRenderer
{
	public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();
	void DrawSprite(Texture* texture, glm::vec2 position, glm::vec2 size = glm::vec2(10, 10), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	Shader* rendererShader;
	GLuint quadVAO;
	void initRenderData();
};

