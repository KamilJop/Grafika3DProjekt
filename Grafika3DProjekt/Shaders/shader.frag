#version 330

in vec4 vCol;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

struct Light
{
	vec3 lightColor;
	vec3 lightDirection;
	float lightAmbientIntensity;
	float lightDiffuseIntensity;
};

uniform Light light;


void main()
{
	vec4 lightAmbientColor = vec4(light.lightColor, 1.0f) * light.lightAmbientIntensity;
	float diffuseFactor = max(dot(normalize(Normal), normalize(-light.lightDirection)), 0.0f);
	vec4 lightDiffuseColor = vec4(light.lightColor, 1.0f) * light.lightDiffuseIntensity * diffuseFactor;
	
	colour = (lightAmbientColor + lightDiffuseColor) * vCol;
}