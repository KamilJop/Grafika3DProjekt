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
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace);
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
		// Insert vertices
		vertices.insert(vertices.end(), {mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z});
		// Insert normals
		vertices.insert(vertices.end(), { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z });
		// Insert texture coordinates
		if (mesh->mTextureCoords[0]) {
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		// Insert tangents
		vertices.insert(vertices.end(), { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z });

		// Insert bitangents
		vertices.insert(vertices.end(), { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z });
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
	newMesh->CreateMesh(&vertices[0], &indices[0], (unsigned int)vertices.size(), (unsigned int)indices.size(), 14);
	meshes.push_back(newMesh);
	meshToTexture.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	textures.resize(scene->mNumMaterials);
	normalMaps.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		textures[i] = nullptr;
		normalMaps[i] = nullptr;

		aiString path;
		bool textureFound = false;

		// Try loading the diffuse texture
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
			textureFound = true;
		}
		// Try loading the base color texture (PBR)
		else if (material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS) {
			textureFound = true;
		}

		// If a texture path was found, load the texture
		if (textureFound) {
			// Get the filename from the full path
			std::string fullpath_s = path.C_Str();
			std::string filename;
			// Find the last slash or backslash (in case of Windows paths in downloaded models)
			size_t lastSlash = fullpath_s.find_last_of("/\\");

			// Extract the filename
			if (lastSlash == std::string::npos) {
				filename = fullpath_s;
			}
			else {
				filename = fullpath_s.substr(lastSlash + 1);
			}
			// Create the full path to the texture
			std::string fullpath = "Textures/" + filename;

			// Load the texture
			textures[i] = new Texture(fullpath.c_str()); 

			// Error handling
			if (!textures[i]->LoadTextureAlpha()) {
				printf("Failed to load texture: %s\n", fullpath.c_str());
				fflush(stdout);
				delete textures[i];
				textures[i] = nullptr;
			}
		}
		// If no texture was found, use a default texture
		if (!textures[i]) {
			textures[i] = new Texture("Textures/stone.png");
			textures[i]->LoadTextureAlpha();
		}
		// Try loading the normal map
		aiString normalPath;
		textureFound = false;
		
		if(material ->GetTexture(aiTextureType_NORMALS, 0, &normalPath) == AI_SUCCESS) {
			textureFound = true;
		}
		if(material ->GetTexture(aiTextureType_HEIGHT, 0, &normalPath) == AI_SUCCESS) {
			textureFound = true;
		}

		if(textureFound){
			// Get the filename from the full path
			std::string fullpath_s = normalPath.C_Str();
			std::string filename;
			// Find the last slash or backslash (in case of Windows paths in downloaded models)
			size_t lastSlash = fullpath_s.find_last_of("/\\");
			// Extract the filename
			if (lastSlash == std::string::npos) {
				filename = fullpath_s;
			}
			else {
				filename = fullpath_s.substr(lastSlash + 1);
			}
			// Create the full path to the texture
			std::string fullpath = "Textures/" + filename;
			// Load the texture
			normalMaps[i] = new Texture(fullpath.c_str());
			// Error handling
			if (!normalMaps[i]->LoadNormalMap()) {
				printf("Failed to load normal map: %s\n", fullpath.c_str());
				fflush(stdout);
				delete normalMaps[i];
				normalMaps[i] = nullptr;
			}
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

	for (size_t i = 0; i < normalMaps.size(); i++) {
		if (normalMaps[i]) {
			normalMaps[i]->ClearTexture();
			delete normalMaps[i];
			normalMaps[i] = nullptr;
		}
	}
	normalMaps.clear();
}

void Model::RenderModel()
{
	for (size_t i = 0; i < meshes.size(); i++) {
		unsigned int materialIndex = meshToTexture[i];
		if (materialIndex < textures.size() && textures[materialIndex]) {
			textures[materialIndex]->UseTexture(GL_TEXTURE0);
		}
		if (materialIndex < normalMaps.size() && normalMaps[materialIndex]) {
			normalMaps[materialIndex]->UseTexture(GL_TEXTURE3);
		}
		meshes[i]->RenderMesh();
	}
}