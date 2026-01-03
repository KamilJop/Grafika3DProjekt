#include "Model.h"

/**
 * @brief Creates an empty model.
 */
Model::Model()
{
}

/**
 * @brief Destructor clears all meshes, textures, and materials.
 */
Model::~Model()
{
    ClearModel();
}

/**
 * @brief Loads a 3D model from file using Assimp.
 *
 * Steps:
 * - Import scene
 * - Recursively load all meshes
 * - Load materials and textures
 * - Compute collision bounding box
 *
 * @param path File path to the model.
 */
void Model::LoadModel(const std::string& path)
{
    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_CalcTangentSpace |
        aiProcess_GenBoundingBoxes
    );

    if (!scene)
    {
        printf("Model failed to load: %s\n", importer.GetErrorString());
        fflush(stdout);
        return;
    }

    LoadNode(scene->mRootNode, scene);
    LoadMaterials(scene);
    LoadCollisionBox(scene);
}

/**
 * @brief Recursively loads all meshes from a node.
 *
 * @param node Current Assimp node.
 * @param scene Full Assimp scene.
 */
void Model::LoadNode(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        LoadMesh(mesh, scene);
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        LoadNode(node->mChildren[i], scene);
    }
}

/**
 * @brief Converts an Assimp mesh into engine mesh format.
 *
 * Extracts:
 * - Positions
 * - Normals
 * - UVs
 * - Tangents
 * - Bitangents
 *
 * @param mesh Assimp mesh.
 * @param scene Full Assimp scene.
 */
void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.insert(vertices.end(),
        {
            mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z,
            mesh->mNormals[i].x,  mesh->mNormals[i].y,  mesh->mNormals[i].z
        });

        if (mesh->mTextureCoords[0])
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        else
            vertices.insert(vertices.end(), { 0.0f, 0.0f });

        vertices.insert(vertices.end(),
        {
            mesh->mTangents[i].x,   mesh->mTangents[i].y,   mesh->mTangents[i].z,
            mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z
        });
    }

    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Mesh* newMesh = new Mesh();
    newMesh->CreateMesh(&vertices[0], &indices[0],
                        (unsigned int)vertices.size(),
                        (unsigned int)indices.size(),
                        14);

    meshes.push_back(newMesh);
    meshToTexture.push_back(mesh->mMaterialIndex);
}

/**
 * @brief Loads textures, normal maps, parallax maps, and material properties.
 *
 * Supports:
 * - Diffuse / Base Color textures
 * - Normal maps
 * - Parallax maps
 * - Specular intensity
 * - Shininess
 *
 * @param scene Full Assimp scene.
 */
void Model::LoadMaterials(const aiScene* scene)
{
    textures.resize(scene->mNumMaterials);
    normalMaps.resize(scene->mNumMaterials);
    paralaxMaps.resize(scene->mNumMaterials);
    materials.resize(scene->mNumMaterials);

    for (size_t i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* material = scene->mMaterials[i];
        textures[i] = nullptr;
        normalMaps[i] = nullptr;

        float shininess = 32.0f;
        float specularIntensity = 0.5f;

        material->Get(AI_MATKEY_SHININESS, shininess);
        aiColor4D specColor;
        if (material->Get(AI_MATKEY_COLOR_SPECULAR, specColor) == AI_SUCCESS)
            specularIntensity = (specColor.r + specColor.g + specColor.b) / 3.0f;

        materials[i] = new Material(specularIntensity, shininess);

        aiString path;
        bool textureFound = false;

        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS ||
            material->GetTexture(aiTextureType_BASE_COLOR, 0, &path) == AI_SUCCESS)
        {
            textureFound = true;
        }

        if (textureFound)
        {
            std::string fullpath_s = path.C_Str();
            size_t lastSlash = fullpath_s.find_last_of("/\\");
            std::string filename = (lastSlash == std::string::npos)
                ? fullpath_s
                : fullpath_s.substr(lastSlash + 1);

            std::string fullpath = "Textures/" + filename;

            textures[i] = new Texture(fullpath.c_str());
            if (!textures[i]->LoadTextureAlpha())
            {
                printf("Failed to load texture: %s\n", fullpath.c_str());
                delete textures[i];
                textures[i] = nullptr;
            }
        }

        if (!textures[i])
        {
            textures[i] = new Texture("Textures/stone.png");
            textures[i]->LoadTextureAlpha();
        }

        // Normal map loading
        aiString normalPath;
        textureFound = material->GetTexture(aiTextureType_NORMALS, 0, &normalPath) == AI_SUCCESS ||
                       material->GetTexture(aiTextureType_HEIGHT, 0, &normalPath) == AI_SUCCESS;

        if (textureFound)
        {
            std::string fullpath_s = normalPath.C_Str();
            size_t lastSlash = fullpath_s.find_last_of("/\\");
            std::string filename = (lastSlash == std::string::npos)
                ? fullpath_s
                : fullpath_s.substr(lastSlash + 1);

            std::string fullpath = "Textures/" + filename;

            normalMaps[i] = new Texture(fullpath.c_str());
            if (!normalMaps[i]->LoadMaps())
            {
                printf("Failed to load normal map: %s\n", fullpath.c_str());
                delete normalMaps[i];
                normalMaps[i] = nullptr;
            }
        }

        if (!normalMaps[i])
        {
            normalMaps[i] = new Texture("Textures/default_normal.png");
            normalMaps[i]->LoadMaps();
        }

        // Parallax map loading
        aiString parallaxPath;
        textureFound = material->GetTexture(aiTextureType_DISPLACEMENT, 0, &parallaxPath) == AI_SUCCESS;

        if (textureFound)
        {
            std::string fullpath_s = parallaxPath.C_Str();
            size_t lastSlash = fullpath_s.find_last_of("/\\");
            std::string filename = (lastSlash == std::string::npos)
                ? fullpath_s
                : fullpath_s.substr(lastSlash + 1);

            std::string fullpath = "Textures/" + filename;

            paralaxMaps[i] = new Texture(fullpath.c_str());
            if (!paralaxMaps[i]->LoadMaps())
            {
                printf("Failed to load paralax map: %s\n", fullpath.c_str());
                delete paralaxMaps[i];
                paralaxMaps[i] = nullptr;
            }
        }

        if (!paralaxMaps[i])
        {
            paralaxMaps[i] = new Texture("Textures/default_height.png");
            paralaxMaps[i]->LoadMaps();
        }
    }
}

/**
 * @brief Computes the model's axis-aligned bounding box.
 *
 * Uses Assimp-generated AABBs from each mesh.
 *
 * @param scene Full Assimp scene.
 */
void Model::LoadCollisionBox(const aiScene* scene)
{
    if (scene->mNumMeshes == 0)
        return;

    const aiAABB& first = scene->mMeshes[0]->mAABB;
    collisionBox.min = glm::vec3(first.mMin.x, first.mMin.y, first.mMin.z);
    collisionBox.max = glm::vec3(first.mMax.x, first.mMax.y, first.mMax.z);

    for (size_t i = 1; i < scene->mNumMeshes; i++)
    {
        const aiAABB& aabb = scene->mMeshes[i]->mAABB;

        collisionBox.min.x = std::min(collisionBox.min.x, aabb.mMin.x);
        collisionBox.min.y = std::min(collisionBox.min.y, aabb.mMin.y);
        collisionBox.min.z = std::min(collisionBox.min.z, aabb.mMin.z);

        collisionBox.max.x = std::max(collisionBox.max.x, aabb.mMax.x);
        collisionBox.max.y = std::max(collisionBox.max.y, aabb.mMax.y);
        collisionBox.max.z = std::max(collisionBox.max.z, aabb.mMax.z);
    }

    printf("Collision Box Min: (%f, %f, %f)\n",
           collisionBox.min.x, collisionBox.min.y, collisionBox.min.z);
}

/**
 * @brief Frees all meshes, textures, and materials.
 */
void Model::ClearModel()
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        if (meshes[i])
        {
            meshes[i]->ClearMesh();
            delete meshes[i];
            meshes[i] = nullptr;
        }
    }
    meshes.clear();

    for (size_t i = 0; i < textures.size(); i++)
    {
        if (textures[i])
        {
            textures[i]->ClearTexture();
            delete textures[i];
            textures[i] = nullptr;
        }
    }
    textures.clear();

    meshToTexture.clear();

    for (size_t i = 0; i < normalMaps.size(); i++)
    {
        if (normalMaps[i])
        {
            normalMaps[i]->ClearTexture();
            delete normalMaps[i];
            normalMaps[i] = nullptr;
        }
    }
    normalMaps.clear();
}

/**
 * @brief Renders all meshes with their associated textures and materials.
 *
 * @param shader Shader used for rendering.
 */
void Model::RenderModel(Shader* shader)
{
    for (size_t i = 0; i < meshes.size(); i++)
    {
        unsigned int materialIndex = meshToTexture[i];

        if (materialIndex < textures.size() && textures[materialIndex])
            textures[materialIndex]->UseTexture(GL_TEXTURE0);

        if (materialIndex < normalMaps.size() && normalMaps[materialIndex])
            normalMaps[materialIndex]->UseTexture(GL_TEXTURE3);

        if (materialIndex < paralaxMaps.size() && paralaxMaps[materialIndex])
            paralaxMaps[materialIndex]->UseTexture(GL_TEXTURE4);

        if (materialIndex < materials.size() && materials[materialIndex])
            materials[materialIndex]->useMaterial(shader);

        meshes[i]->RenderMesh();
    }
}
