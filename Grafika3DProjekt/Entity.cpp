#include "Entity.h"


// Constructor
Entity::Entity(Model* model,Material* material, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal)
{
	position = pos;
	rotation = rot;
	scale = scal;
	entityModel = model;
	entityMaterial = material;
	modelMatrix = CalculateModelMatrix();
	collisions = model->GetCollisionBox();

	// Update collision box
	UpdateCollisionBox();
}


// Destructor
Entity::~Entity()
{
	position = glm::vec3(0.0f);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
	entityModel = nullptr;
	entityMaterial = nullptr;
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
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Apply scaling
	model = glm::scale(model, scale);
	return model;
}

// Draw the entity
void Entity::DrawEntity(Shader* shader)
{
	// Recalculate model matrix
	modelMatrix = CalculateModelMatrix();

	// Send the model matrix to the shader
	shader->setMat4("model", modelMatrix);

	// Set material properties in shader
	entityMaterial->useMaterial(shader);

	// Draw the models
	entityModel->RenderModel();

}

void Entity::UpdateCollisionBox()
{
	collisions.min = position + entityModel->GetCollisionBox().min * scale;
	collisions.max = position + entityModel->GetCollisionBox().max * scale;
	printf("Collision Box Updated: Min(%.2f, %.2f, %.2f) Max(%.2f, %.2f, %.2f)\n",
		collisions.min.x, collisions.min.y, collisions.min.z,
		collisions.max.x, collisions.max.y, collisions.max.z);
}