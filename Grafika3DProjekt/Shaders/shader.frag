#version 330

in vec4 vCol;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

struct DirectionalLight
{
	vec3 lightColor;
	vec3 lightDirection;
	float lightAmbientIntensity;
	float lightDiffuseIntensity;
};

struct PointLight
{
	vec3 lightColor;
	vec3 lightPosition;
	float lightAmbientIntensity;
	float lightDiffuseIntensity;
	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	float specularIntensity;
	float shininess;
};


uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform Material material;

uniform vec3 cameraPosition;



void main()
{

	// Ambient light in DirectionalLight
	
	vec4 lightAmbientColor = vec4(directionalLight.lightColor, 1.0f) * directionalLight.lightAmbientIntensity;
	
	// Diffuse light in DirectionalLight
	
	// Calculate angle between the Normal and light direction
	float diffuseFactor = max(dot(normalize(Normal), normalize(-directionalLight.lightDirection)), 0.0f);
	
	// Calculate diffuse color based on previous angle
	vec4 lightDiffuseColor = vec4(directionalLight.lightColor, 1.0f) * directionalLight.lightDiffuseIntensity * diffuseFactor;
	
	// Specular light in DirectionalLight
	
	// Initialize empty vec4 for specular light
	vec4 lightSpecularColor = vec4(0.0f);
	
	// Check if specular light is needed on surface
	if(diffuseFactor > 0.0f)
	{
		// Calculate direction we're looking at fragment with
		vec3 fragToCam = normalize(cameraPosition - FragPos);
		
		// Calculate vector of reflected light
		vec3 reflectedVertex = normalize(reflect(directionalLight.lightDirection, normalize(Normal)));
		
		// Calculate angle between our eye and reflected light 
		float specularFactor = dot(fragToCam, reflectedVertex);
		
		if(specularFactor > 0.0f)
		{	
			// Multiply the factor by material properties
			specularFactor = pow(specularFactor, material.shininess);
			// Calculate specular color 
			lightSpecularColor = vec4(directionalLight.lightColor * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	// Calculate vector between point light source and current fragment
	vec3 pointLightDistanceVector = pointLight.lightPosition - FragPos;
	
	// Calculate distance between point light source and current fragment
	float distance = length(pointLightDistanceVector);
	
	// Calculate direction vector between point light source and current fragment
	vec3 pointLightDirection = normalize(pointLightDistanceVector);
	
	// Calculate attenuation using formula
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
	
	// Calculate ambient color and multiply by attenuation
	vec4 pointLightAmbientColor = vec4(pointLight.lightColor, 1.0f) * pointLight.lightAmbientIntensity * attenuation;
	
	// Calculate diffuse color and multiply by attenuation
	float pointDiffuseFactor = max(dot(Normal, pointLightDirection), 0.0f);
	vec4 pointLightDiffuseColor = vec4(pointLight.lightColor, 1.0f) * pointLight.lightDiffuseIntensity * pointDiffuseFactor * attenuation;
	
	// Initialize empty vec4 for specular lightning
	vec4 pointLightSpecularColor = vec4(0.0f);
	
	// Check if specular light is neccesery 
	if (pointDiffuseFactor > 0.0f){
		// Calculate direction we're looking at fragment with
		vec3 fragToCam = normalize(cameraPosition - FragPos);
		
		// Calculate the vector of reflected light
		vec3 reflectedVertex = normalize(reflect(-pointLightDirection, Normal));
		
		// Calculate angle between our eye and reflected light 
		float pointSpecularFactor = dot(fragToCam, reflectedVertex);
		if(pointSpecularFactor > 0.0f)
		{
			// Multiply the factor by material properties
			pointSpecularFactor = pow(pointSpecularFactor, material.shininess);
			
			// Calculate specular color and multiply it by attenuation
			pointLightSpecularColor = vec4(pointLight.lightColor * material.specularIntensity * pointSpecularFactor, 1.0f) * attenuation;
		}
	}
	
	// Summmarize all light colors together
	vec4 finalLightColor = (lightAmbientColor + lightDiffuseColor + lightSpecularColor) + (pointLightAmbientColor + pointLightDiffuseColor + pointLightSpecularColor);
	
	// Multiply the color by light 
	colour = finalLightColor * vCol;
}