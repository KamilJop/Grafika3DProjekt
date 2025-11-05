#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Mesh.h"
#include "Shader.h"0


class Entity
{
public:

	Mesh* entinyMesh;
	Shader* entityShader;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Entity(Mesh* mesh, Shader* shader, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

	~Entity();

	glm::mat4 GetModelMatrix();

private:
	glm::mat4 modelMatrix;

	glm::mat4 CalculateModelMatrix();
	void DrawEntity();

};

