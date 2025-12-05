#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include "Mesh.h"
#include "Texture.h"
#include "CollisionBox.h"
#include "Material.h"


class Model
{
public:

	Model();
	~Model();
	void LoadModel(const std::string& path);
	void RenderModel(Shader* shader);
	void ClearModel();
	CollisionBox GetCollisionBox() { return collisionBox; }

private:

	void LoadNode(aiNode* node, const aiScene* scene);
	void LoadMesh(aiMesh* mesh, const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
	void LoadCollisionBox(const aiScene* scene);

	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures;
	std::vector<Texture*> normalMaps;
	std::vector<Texture*> paralaxMaps;
	std::vector<Material*> materials;

	std::vector<unsigned int> meshToTexture;
	std::vector<unsigned int> meshToMaterial;
	std::string directory;

	CollisionBox collisionBox;
};

