#pragma once
#include <iostream>
#include <glad/glad.h>

/**
 * @class HDRBuffer
 * @brief High‑dynamic‑range framebuffer with bloom support.
 *
 * Provides:
 * - Two floating‑point color buffers (scene + bright pass)
 * - Depth/stencil renderbuffer
 * - Ping‑pong framebuffers for Gaussian blur
 * - A fullscreen quad for post‑processing
 */
class HDRBuffer
{
public:

    /**
     * @brief Creates an HDR framebuffer and initializes all attachments.
     *
     * @param width  Framebuffer width.
     * @param height Framebuffer height.
     */
    HDRBuffer(int width, int height);

    /** @brief Destructor cleans up all OpenGL resources. */
    ~HDRBuffer();

    /** @brief Binds the HDR framebuffer for rendering. */
    void Bind();

    /** @brief Unbinds the framebuffer (returns to default). */
    void Unbind();

    /** @brief Returns the main HDR color buffer. */
    unsigned int getColorBuffer() { return colorBuffers[0]; }

    /** @brief Returns the bright-pass texture used for bloom. */
    unsigned int getBrightTexture() { return colorBuffers[1]; }

    /** @brief Returns one of the ping‑pong framebuffers. */
    unsigned int getPingPongFBO(int index) { return pingPongFBO[index]; }

    /** @brief Returns one of the ping‑pong color buffers. */
    unsigned int getPingPongColorBuffer(int index) { return pingPongColorbuffers[index]; }

    /** @brief Renders the fullscreen quad (used for tone mapping and blur). */
    void RenderQuad();

private:
    unsigned int FBO;                 ///< HDR framebuffer.
    unsigned int colorBuffers[2];     ///< Scene + bright-pass textures.

    unsigned int pingPongFBO[2];      ///< Blur framebuffers.
    unsigned int pingPongColorbuffers[2]; ///< Blur textures.

    unsigned int RBO;                 ///< Depth/stencil renderbuffer.

    unsigned int quadVAO = 0;         ///< Fullscreen quad VAO.
    unsigned int quadVBO;             ///< Fullscreen quad VBO.

    /** @brief Initializes the fullscreen quad geometry. */
    void InitQuad();
};
