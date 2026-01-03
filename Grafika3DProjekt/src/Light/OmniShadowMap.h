#pragma once
#include "ShadowMap.h"

/**
 * @class OmniShadowMap
 * @brief Cube‑map shadow map used for point lights.
 *
 * Extends ShadowMap to create a depth-only cube map
 * that captures shadows in all directions.
 */
class OmniShadowMap : public ShadowMap
{
public:

    /** @brief Creates an empty omni-directional shadow map. */
    OmniShadowMap();

    /** @brief Destructor. */
    ~OmniShadowMap();

    /**
     * @brief Initializes the cube-map shadow framebuffer and textures.
     *
     * @param width Shadow map width.
     * @param height Shadow map height.
     * @return True if initialization succeeded.
     */
    bool Init(GLuint width, GLuint height);

    /**
     * @brief Binds the cube-map texture for reading in shaders.
     *
     * @param textureUnit Texture unit to bind to.
     */
    void Read(GLenum textureUnit);

    /**
     * @brief Binds the framebuffer for writing depth values.
     */
    void Write();
};
