#include "OmniShadowMap.h"

OmniShadowMap::OmniShadowMap() : ShadowMap() {};

bool OmniShadowMap::Init(GLuint width, GLuint height) {
	// Set shadow map dimensions
	shadowWidth = width;
	shadowHeight = height;

	// Generate Framebuffer
	glGenFramebuffers(1, &FBO);

	// Generate and bind the depth texture as cube map
	glGenTextures(1, &shadowMap);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);

	// Configure depth texture for each face of the cube map (positive x, negative x, positive y, negative y, positive z, negative z)
	for(size_t i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	}

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach depth texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);

	// Disable color buffer writes
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Check for errors
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer not complete!\n");
		return false;
	}

	return true;
}

void OmniShadowMap::Read(GLenum textureUnit) {
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, shadowMap);
}

void OmniShadowMap::Write() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
}