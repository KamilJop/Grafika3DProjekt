#pragma once

#include <stdio.h>
#include <glad\glad.h>
class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();
	virtual bool Init(GLuint width, GLuint height);
	virtual void Write();
	virtual void Read(GLenum textureUnit);
	GLuint getShadowWidth() { return shadowWidth; }
	GLuint getShadowHeight() { return shadowHeight; }
protected:
	GLuint FBO;
	GLuint shadowMap;
	GLuint shadowWidth;
	GLuint shadowHeight;

};

