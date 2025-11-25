#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Texture.h"

struct CollisionBox
{
	glm::vec3 min;
	glm::vec3 max;
};

class Model
{
public:

	Model();
	~Model();
	void LoadModel(const std::string& path);
	void RenderModel();
	void ClearModel();
private:

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
	void LoadCollisionBox(const aiScene* scene);

	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures;
	std::vector<Texture*> normalMaps;
	std::vector<Texture*> paralaxMaps;

	std::vector<unsigned int> meshToTexture;
	std::string directory;

	CollisionBox collisionBox;
};

