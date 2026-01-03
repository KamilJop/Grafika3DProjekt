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

/**
 * @class Model
 * @brief Loads and renders 3D models using Assimp.
 *
 * Handles:
 * - Mesh loading and creation
 * - Material and texture loading (diffuse, normal, parallax)
 * - Collision box generation
 * - Rendering with associated materials
 */
class Model
{
public:

    /** @brief Creates an empty model. */
    Model();

    /** @brief Destructor clears all meshes, textures, and materials. */
    ~Model();

    /**
     * @brief Loads a model from file using Assimp.
     *
     * @param path Path to the model file.
     */
    void LoadModel(const std::string& path);

    /**
     * @brief Renders all meshes with their textures and materials.
     *
     * @param shader Shader used for rendering.
     */
    void RenderModel(Shader* shader);

    /** @brief Frees all loaded meshes, textures, and materials. */
    void ClearModel();

    /** @brief Returns the model's axis‑aligned bounding box. */
    CollisionBox GetCollisionBox() { return collisionBox; }

private:

    /** @brief Recursively loads meshes from an Assimp node. */
    void LoadNode(aiNode* node, const aiScene* scene);

    /** @brief Converts an Assimp mesh into an engine mesh. */
    void LoadMesh(aiMesh* mesh, const aiScene* scene);

    /** @brief Loads textures, normal maps, parallax maps, and materials. */
    void LoadMaterials(const aiScene* scene);

    /** @brief Computes the model's axis‑aligned bounding box. */
    void LoadCollisionBox(const aiScene* scene);

    std::vector<Mesh*> meshes;            ///< Meshes in the model.
    std::vector<Texture*> textures;       ///< Diffuse/base color textures.
    std::vector<Texture*> normalMaps;     ///< Normal maps.
    std::vector<Texture*> paralaxMaps;    ///< Parallax/height maps.
    std::vector<Material*> materials;     ///< Material properties.

    std::vector<unsigned int> meshToTexture;  ///< Maps mesh index to texture index.
    std::vector<unsigned int> meshToMaterial; ///< Maps mesh index to material index.

    std::string directory;                ///< Directory of the loaded model.

    CollisionBox collisionBox;            ///< Axis‑aligned bounding box.
};
