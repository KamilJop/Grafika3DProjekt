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

struct Material
{
	float specularIntensity;
	float shininess;
};


uniform Light light;
uniform Material material;

uniform vec3 cameraPosition;



void main()
{
	vec4 lightAmbientColor = vec4(light.lightColor, 1.0f) * light.lightAmbientIntensity;
	
	float diffuseFactor = max(dot(normalize(Normal), normalize(-light.lightDirection)), 0.0f);
	vec4 lightDiffuseColor = vec4(light.lightColor, 1.0f) * light.lightDiffuseIntensity * diffuseFactor;
	
	vec4 lightSpecularColor = vec4(0,0,0,0);
	
	if(diffuseFactor > 0.0f)
	{
		vec3 fragToCam = normalize(cameraPosition - FragPos);
		vec3 reflectedVertex = normalize(reflect(light.lightDirection, normalize(Normal)));
		float specularFactor = dot(fragToCam, reflectedVertex);
		if(specularFactor > 0.0f)
		{
			specularFactor = pow(specularFactor, material.shininess);
			lightSpecularColor = vec4(light.lightColor * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	colour = (lightAmbientColor + lightDiffuseColor + lightSpecularColor) * vCol;
}