#version 330

in vec4 vCol;

out vec4 colour;

struct Light
{
	vec3 lightColor;
	vec3 lightPos;
	float lightAmbientIntensity;
};

uniform Light light;

void main()
{
	vec4 lightAmbientColor = vec4(light.lightColor, 1.0f) * light.lightAmbientIntensity;
	colour = vCol * lightAmbientColor;
}