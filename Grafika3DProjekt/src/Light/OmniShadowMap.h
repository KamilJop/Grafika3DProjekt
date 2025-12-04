#pragma once
#include "ShadowMap.h"
class OmniShadowMap : public ShadowMap
{
public:
	OmniShadowMap();
	~OmniShadowMap();
	bool Init(GLuint width, GLuint height);
	void Read(GLenum textureUnit) ;
	void Write();
};

