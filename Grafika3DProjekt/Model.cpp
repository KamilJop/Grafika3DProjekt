#include "Model.h"

// Constructor
Model::Model()
{
}

// Destructor
Model::~Model()
{
	ClearModel();
}

void Model::LoadModel(const std::string& path)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// Read the model file
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	// Check for errors
	if (!scene)
	{
		printf("Model failed to load: %s\n", importer.GetErrorString());
		fflush(stdout);
		return;
	}
	// Load the root node
	LoadNode(scene->mRootNode, scene);
	// Load materials
	LoadMaterials(scene);
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		LoadMesh(mesh, scene);
	}
	// Recurse for each child node
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	// Process vertices
	for (size_t i = 0; i < mesh->mNumVertices; i++) {
		vertices.insert(vertices.end(), {mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z});
		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
	}

	// Process indices
	for (size_t i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Create mesh
	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], (unsigned int)vertices.size(), (unsigned int)indices.size(), 8);
	meshes.push_back(newMesh);
	meshToTexture.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textures.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		textures[i] = nullptr;

		aiString path;
		bool textureFound = false;

		// 1. Spróbuj za³adowaæ standardow¹ teksturê DIFFUSE
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			textureFound = true;
		}
		// 2. Jeœli nie ma DIFFUSE, spróbuj za³adowaæ teksturê BASE_COLOR (dla modeli PBR)
		else if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS) {
			textureFound = true;
		}

		if (textureFound) {
			std::string fullpath_s = path.C_Str();
			std::string filename;
			size_t lastSlash = fullpath_s.find_last_of("/\\");

			if (lastSlash == std::string::npos) {
				filename = fullpath_s;
			}
			else {
				filename = fullpath_s.substr(lastSlash + 1);
			}
			std::string fullpath = "Textures/" + filename;

			textures[i] = new Texture(fullpath.c_str()); // Upewnij siê, ¿e Texture(const char*) robi kopiê!

			if (!textures[i]->LoadTextureAlpha()) {
				printf("Failed to load texture: %s\n", fullpath.c_str());
				fflush(stdout);
				delete textures[i];
				textures[i] = nullptr;
			}
		}

		if (!textures[i]) {
			textures[i] = new Texture("Textures/stone.png");
			textures[i]->LoadTextureAlpha();
		}
	}
}
void Model::ClearModel()
{
	for (size_t i = 0; i < meshes.size(); i++) {
		if (meshes[i]) {
			meshes[i]->ClearMesh();
			delete meshes[i];
			meshes[i] = nullptr;
		}
	}
	meshes.clear();
	for (size_t i = 0; i < textures.size(); i++) {
		if (textures[i]) {
			textures[i]->ClearTexture();
			delete textures[i];
			textures[i] = nullptr;
		}
	}
	textures.clear();
	meshToTexture.clear();
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshes.size(); i++) {
		unsigned int materialIndex = meshToTexture[i];
		if (materialIndex < textures.size() && textures[materialIndex]) {
			textures[materialIndex]->UseTexture();
		}
		meshes[i]->RenderMesh();
	}
}