#include "Entity.h"


// Constructor
Entity::Entity(Mesh* mesh, Shader* shader, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal)
{
	entityMesh = mesh;
	entityShader = shader;
	position = pos;
	rotation = rot;
	scale = scal;
	modelMatrix = CalculateModelMatrix();
}


// Destructor
Entity::~Entity()
{
	entityMesh = nullptr;
	entityShader = nullptr;
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

// Getter for model matrix
glm::mat4 Entity::GetModelMatrix()
{	
	// Recalculate model matrix
	modelMatrix = CalculateModelMatrix();

	return modelMatrix;
}

// Setter position
void Entity::setPosition(glm::vec3 newPosition)
{
	position = newPosition;
}

// Setter rotation
void Entity::setRotation(glm::vec3 newRotation)
{
	rotation = newRotation;
}

// Setter scale
void Entity::setScale(glm::vec3 newScale)
{
	scale = newScale;
}

// Getter position
glm::vec3 Entity::getPosition()
{
	return position;
}

// Getter rotation
glm::vec3 Entity::getRotation()
{
	return rotation;
}

// Getter scale
glm::vec3 Entity::getScale()
{
	return scale;
}

// Calculate model matrix
glm::mat4 Entity::CalculateModelMatrix()
{
	glm::mat4 model(1.0f);

	// Apply position translation
	model = glm::translate(model, position);

	// Apply rotations
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Apply scaling
	model = glm::scale(model, scale);
	return model;
}

// Draw the entity
void Entity::DrawEntity()
{
	// Send the model matrix to the shader
	entityShader->setMat4("model", modelMatrix);


	// Render the mesh
	entityMesh->RenderMesh();
}