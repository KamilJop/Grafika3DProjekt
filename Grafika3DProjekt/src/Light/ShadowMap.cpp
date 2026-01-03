#include "ShadowMap.h"

/**
 * @brief Creates an empty shadow map object.
 *
 * Initializes all OpenGL handles and dimensions to zero.
 */
ShadowMap::ShadowMap()
{
    FBO = 0;
    shadowMap = 0;
    shadowWidth = 0;
    shadowHeight = 0;
}

/**
 * @brief Destructor cleans up framebuffer and texture resources.
 */
ShadowMap::~ShadowMap()
{
    if (FBO)
        glDeleteFramebuffers(1, &FBO);

    if (shadowMap)
        glDeleteTextures(1, &shadowMap);
}

/**
 * @brief Initializes a 2D depth texture and framebuffer for shadow mapping.
 *
 * Creates:
 * - A framebuffer object
 * - A depth-only texture
 *
 * @param width Shadow map width.
 * @param height Shadow map height.
 * @return True if initialization succeeded.
 */
bool ShadowMap::Init(GLuint width, GLuint height)
{
    shadowWidth = width;
    shadowHeight = height;

    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 shadowWidth, shadowHeight, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                     new float[4]{1.0f, 1.0f, 1.0f, 1.0f});

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("Framebuffer error\n");
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

/**
 * @brief Binds the framebuffer for writing depth values.
 */
void ShadowMap::Write()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

/**
 * @brief Binds the shadow map texture for reading in shaders.
 *
 * @param textureUnit Texture unit to bind to.
 */
void ShadowMap::Read(GLenum textureUnit)
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
}
