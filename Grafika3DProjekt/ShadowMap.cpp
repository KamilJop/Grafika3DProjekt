#include "ShadowMap.h"


// Constructor
ShadowMap::ShadowMap()
{
	FBO = 0;
	shadowMap = 0;
	shadowWidth = 0;
	shadowHeight = 0;
}

// Destructor
ShadowMap::~ShadowMap()
{
	if (FBO)
	{
		glDeleteFramebuffers(1, &FBO);
	}
	if (shadowMap)
	{
		glDeleteTextures(1, &shadowMap);
	}
}

// Initialize Shadow Map
bool ShadowMap::Init(GLuint width, GLuint height)
{	
	// Set shadow map dimensions
	shadowWidth = width;
	shadowHeight = height;
	
	// Generate Framebuffer
	glGenFramebuffers(1, &FBO);

	// Generate and bind the depth texture
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);

	// Configure depth texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, new float[4] { 1.0f, 1.0f, 1.0f, 1.0f });
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach depth texture to FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);


	// Disable color buffer writes
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Check for errors
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Framebuffer error: %d\n", status);
		return false;
	}

	// Unbind FBO
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Return true if successful
	return true;
}

// Bind the shadow map for writing
void ShadowMap::Write()
{
	// Bind the FBO
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}	

// Bind the shadow map for reading
void ShadowMap::Read(GLenum textureUnit)
{
	// Activate texture unit and bind the shadow map
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
}

