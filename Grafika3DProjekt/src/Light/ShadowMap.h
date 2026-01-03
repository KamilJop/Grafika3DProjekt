#pragma once

#include <stdio.h>
#include <glad/glad.h>

/**
 * @class ShadowMap
 * @brief Base class for shadow map framebuffers.
 *
 * Provides:
 * - A depth texture
 * - A framebuffer object
 * - Basic read/write binding functions
 *
 * Used by both 2D shadow maps and cube‑map (omni) shadow maps.
 */
class ShadowMap
{
public:

    /** @brief Creates an empty shadow map object. */
    ShadowMap();

    /** @brief Destructor cleans up OpenGL resources. */
    ~ShadowMap();

    /**
     * @brief Initializes a 2D depth texture and framebuffer.
     *
     * @param width Shadow map width.
     * @param height Shadow map height.
     * @return True if initialization succeeded.
     */
    virtual bool Init(GLuint width, GLuint height);

    /**
     * @brief Binds the framebuffer for writing depth values.
     */
    virtual void Write();

    /**
     * @brief Binds the shadow map texture for reading in shaders.
     *
     * @param textureUnit Texture unit to bind to.
     */
    virtual void Read(GLenum textureUnit);

    /** @brief Returns the shadow map width. */
    GLuint getShadowWidth() { return shadowWidth; }

    /** @brief Returns the shadow map height. */
    GLuint getShadowHeight() { return shadowHeight; }

protected:
    GLuint FBO;          ///< Framebuffer object.
    GLuint shadowMap;    ///< Depth texture.
    GLuint shadowWidth;  ///< Shadow map width.
    GLuint shadowHeight; ///< Shadow map height.
};
