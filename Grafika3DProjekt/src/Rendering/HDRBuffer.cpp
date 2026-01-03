#include "HDRBuffer.h"

/**
 * @brief Creates an HDR framebuffer with two floating‑point color attachments.
 *
 * Also sets up:
 * - A depth/stencil renderbuffer
 * - Two ping‑pong framebuffers for Gaussian blur
 * - A fullscreen quad for post‑processing
 *
 * @param width  Framebuffer width.
 * @param height Framebuffer height.
 */
HDRBuffer::HDRBuffer(int width, int height)
{
    // Create main HDR framebuffer
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Two floating‑point color buffers (scene + bright pass)
    glGenTextures(2, colorBuffers);
    for (unsigned int i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
                     width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D, colorBuffers[i], 0);
    }

    // Depth/stencil buffer
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                              GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, RBO);

    // Enable both color attachments
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(2, attachments);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "HDR Framebuffer Error!" << std::endl;

    // Ping‑pong buffers for blurring
    glGenFramebuffers(2, pingPongFBO);
    glGenTextures(2, pingPongColorbuffers);

    for (unsigned int i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, pingPongFBO[i]);
        glBindTexture(GL_TEXTURE_2D, pingPongColorbuffers[i]);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
                     width, height, 0, GL_RGBA, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D, pingPongColorbuffers[i], 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Fullscreen quad for post‑processing
    InitQuad();
}

/**
 * @brief Binds the HDR framebuffer for rendering.
 */
void HDRBuffer::Bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

/**
 * @brief Unbinds the framebuffer (returns to default).
 */
void HDRBuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * @brief Creates a fullscreen quad VAO/VBO for post‑processing passes.
 */
void HDRBuffer::InitQuad()
{
    float quadVertices[] = {
        // positions   // UVs
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices),
                 quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                          5 * sizeof(float), (void*)(3 * sizeof(float)));
}

/**
 * @brief Cleans up all framebuffer, texture, and quad resources.
 */
HDRBuffer::~HDRBuffer()
{
    glDeleteFramebuffers(1, &FBO);
    glDeleteTextures(1, &colorBuffers[0]);
    glDeleteRenderbuffers(1, &RBO);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

/**
 * @brief Renders the fullscreen quad (used for tone mapping and blur passes).
 */
void HDRBuffer::RenderQuad()
{
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}
