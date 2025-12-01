#include "Entity.h"


// Constructor
Entity::Entity(Model* model,Material* material, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
{
	position = pos;
	rotation = rot;
	scale = scal;
	entityModel = model;
	entityMaterial = material;
	modelMatrix = CalculateModelMatrix();
	collisions = model->GetCollisionBox();
	interactable = interaction;
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
	// Get model matrix
	glm::mat4 modelMatrix = GetModelMatrix();

	// Copy local collision box corners
	glm::vec3 localMin = entityModel->GetCollisionBox().min;
	glm::vec3 localMax = entityModel->GetCollisionBox().max;

	// Setup array for 8 corners of the AABB
	glm::vec3 corners[8];
	corners[0] = glm::vec3(localMin.x, localMin.y, localMin.z);
	corners[1] = glm::vec3(localMin.x, localMin.y, localMax.z);
	corners[2] = glm::vec3(localMin.x, localMax.y, localMin.z);
	corners[3] = glm::vec3(localMin.x, localMax.y, localMax.z);
	corners[4] = glm::vec3(localMax.x, localMin.y, localMin.z);
	corners[5] = glm::vec3(localMax.x, localMin.y, localMax.z);
	corners[6] = glm::vec3(localMax.x, localMax.y, localMin.z);
	corners[7] = glm::vec3(localMax.x, localMax.y, localMax.z);

	// Initialize with extreme values
	collisions.min = glm::vec3(std::numeric_limits<float>::max());
	collisions.max = glm::vec3(std::numeric_limits<float>::lowest());

	
	for (int i = 0; i < 8; i++)
	{
		// Transform each corner by model matrix
		glm::vec4 transformedPos = modelMatrix * glm::vec4(corners[i], 1.0f);

		// Calculate new min and max
		collisions.min = glm::min(collisions.min, glm::vec3(transformedPos));
		collisions.max = glm::max(collisions.max, glm::vec3(transformedPos));
	}

}