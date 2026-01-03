#include "OmniShadowMap.h"

/**
 * @brief Creates an omni-directional shadow map (cube map).
 *
 * Inherits from ShadowMap and prepares a cube-map depth texture
 * used for point light shadows.
 */
OmniShadowMap::OmniShadowMap() : ShadowMap() {}

/**
 * @brief Initializes the cube-map shadow framebuffer and textures.
 *
 * Creates:
 * - A framebuffer object
 * - A depth-only cube map (6 faces)
 *
 * @param width Shadow map width.
 * @param height Shadow map height.
 * @return True if initialization succeeded.
 */
bool OmniShadowMap::Init(GLuint width, GLuint height) {
    shadowWidth = width;
    shadowHeight = height;

    glGenFramebuffers(1, &FBO);

    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

    // Allocate depth texture for all 6 cube faces
    for (size_t i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                     GL_DEPTH_COMPONENT, shadowWidth, shadowHeight,
                     0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    }

    // Texture parameters
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach cube map to framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    // Validate framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        printf("Framebuffer not complete!\n");
        return false;
    }

    return true;
}

/**
 * @brief Binds the cube-map texture for reading in shaders.
 *
 * @param textureUnit Texture unit to bind to.
 */
void OmniShadowMap::Read(GLenum textureUnit) {
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}

/**
 * @brief Binds the framebuffer for writing depth values.
 */
void OmniShadowMap::Write() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}

/**
 * @brief Destructor.
 *
 * Calls base ShadowMap destructor.
 */
OmniShadowMap::~OmniShadowMap() {
    ShadowMap::~ShadowMap();
}
