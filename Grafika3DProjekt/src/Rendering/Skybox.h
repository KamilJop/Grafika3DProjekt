#pragma once
#include "External/stb_loader.h"
#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"
#include "Mesh.h"

class Skybox
{
	public:
	Skybox(std::vector<std::string> faces);
	void DrawSkybox(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
	~Skybox();

private:
	Mesh* skyboxMesh;
	Shader* skyboxShader;

	GLuint skyboxTextureID;

};

