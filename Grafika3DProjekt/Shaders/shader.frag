#version 330
#define NR_POINT_LIGHTS 3
in vec4 vCol;
in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoordinates;
in vec4 DirectionalLightSpacePosition;
in vec4 FlashLightSpacePosition;

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

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct FlashLight
{
	vec3 lightColor;
	vec3 lightPosition;
	vec3 lightDirection;
	float ambientIntensity;
	float diffuseIntensity;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};



struct Material
{
	float specularIntensity;
	float shininess;
	sampler2D textureMap;
	sampler2D normalMap;
};


uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform FlashLight flashLight;
uniform Material material;
uniform sampler2D directionalShadowMap;
uniform sampler2D flashShadowMap;
uniform OmniShadowMap omniShadowMaps[NR_POINT_LIGHTS];

uniform vec3 cameraPosition;

// Samples use for omni shadow pcf performance upgrade
vec3 sampleOffsetDirections[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);   


float CalculateDirectionalShadowFactor()
{	

	
	// Get Projection coordinates
	vec3 projCoords = DirectionalLightSpacePosition.xyz / DirectionalLightSpacePosition.w;
	
	// Change values to values between 0 and 1
	projCoords = (projCoords * 0.5) + 0.5;
	
	// Calculating hwo far the fragment is from the light
	float current = projCoords.z;
	
	// Avoiding shadow acne effect
	float bias = 0.001;
	
	// Avoiding pixelating shadows
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(directionalShadowMap,0);
	
	for( int x = -1 ; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(directionalShadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9.0;
	
	
	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
	}
	return shadow;
}

float CalculateFlashLightShadowFactor()
{	

	
	// Get Projection coordinates
	vec3 projCoords = FlashLightSpacePosition.xyz / FlashLightSpacePosition.w;
	
	// Change values to values between 0 and 1
	projCoords = (projCoords * 0.5) + 0.5;
	
	// Calculating hwo far the fragment is from the light
	float current = projCoords.z;
	
	// Avoiding shadow acne effect
	float bias = 0.00001;
	
	// Avoiding pixelating shadows
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(flashShadowMap,0);
	
	for( int x = -1 ; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(flashShadowMap, projCoords.xy + vec2(x,y) * texelSize).r;
			shadow += current - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	
	shadow /= 9.0;
	
	
	if(projCoords.z > 1.0)
	{
		shadow = 0.0;
	}
	return shadow;
}

float CalculateOmniShadowFactor(PointLight pLight, int shadowIndex)
{
	// Calculating vector going from Fragment to Light
	vec3 fragToLight = FragPos - pLight.lightPosition;
	
	
	// Calculate current depth point
	float current = length(fragToLight);
	
	// Avoid shadow acne
	float bias = 0.05;
	float shadow = 0.0;
	int samples = 20;
	float viewDistance = length(cameraPosition - FragPos);
	float diskRadius = (1.0 + (viewDistance/omniShadowMaps[shadowIndex].farPlane)) / 25.0;
	
	for (int i = 0 ; i < samples; i++)
	{
		// Calculate closest 
		float closest = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
		// Multiply to change from 0-1 values to original values
		closest *= omniShadowMaps[shadowIndex].farPlane;
		if (current - bias > closest)
		{
			shadow+=1.0;
		}
	}
	

	
	// Calculate average
	shadow /= float(samples);
	
	return shadow;
}

vec4 CalculateDirectionalLight()
{
	// Calculate shadow Factor
	float shadowFactor = CalculateDirectionalShadowFactor();
	
	// Ambient calculations
	vec4 lightAmbientColor = vec4(directionalLight.lightColor, 1.0f) * directionalLight.lightAmbientIntensity;
	
	// Diffuse calculations 
	// Calculate angle between the Normal and light direction
	float diffuseFactor = max(dot(normalize(Normal), normalize(-directionalLight.lightDirection)), 0.0f);
	// Calculate diffuse color based on diffuse factor
	vec4 lightDiffuseColor = vec4(directionalLight.lightColor, 1.0f) * directionalLight.lightDiffuseIntensity * diffuseFactor;
	
	// Specular calculations
	
	vec4 lightSpecularColor = vec4(0.0f);
	if(diffuseFactor > 0.0f)
	{
		// Calculate direction we're looking at fragment with
		vec3 fragToCam = normalize(cameraPosition - FragPos);
		
		// Calculate halfwayDir for Blinn Phong model
		vec3 halfwayDir = normalize(normalize(-directionalLight.lightDirection) + fragToCam);
		
		// Calculate angle between normal and halfwayDir
		float specularFactor = pow(max(dot(normalize(Normal), halfwayDir), 0.0), material.shininess);
		
		if(specularFactor > 0.0f)
		{	
			lightSpecularColor = vec4(directionalLight.lightColor * material.specularIntensity * specularFactor, 1.0f);
		}
	}
	
	return (lightAmbientColor + (1.0 - shadowFactor) * (lightDiffuseColor + lightSpecularColor));
}


vec4 CalculateFlashLight(){

	float shadowFactor = CalculateFlashLightShadowFactor();
	
	//Initialize black if we're outside the cone
	vec4 flashLightColor = vec4(0.0f);
	
	// Calculate vector between flashlight and fragment
	vec3 flashLightToFragment = flashLight.lightPosition - FragPos;
	
	// Distance from fragment to light
	float flashLightDistance = length(flashLightToFragment);
	
	// Direction from fragment to flashlight
	vec3 flashLightDirection = normalize(flashLightToFragment);
	
	// Count the angle between direction of flashlight and vector calculated line before
	float theta = dot(flashLightDirection, normalize(-flashLight.lightDirection));
	
	// Check if object is inside spotlight cone
	if(theta > flashLight.outerCutOff)
	{
		
		// Calculate attenuation
		float flashAttenuation = 1.0 / (flashLight.constant + flashLight.linear * flashLightDistance + flashLight.quadratic * flashLightDistance * flashLightDistance);
		
		// Calkculate intensity of light in the flashlight
		float epsilon = flashLight.cutOff - flashLight.outerCutOff;
		float spotIntensity = clamp((theta - flashLight.outerCutOff) / epsilon, 0.0, 1.0);
	
		// Calculate ambient lightning for flashLight
		vec3 ambient = flashLight.lightColor * flashLight.ambientIntensity;
		
		// Calculate diffuse lightning for flashlight
		vec3 norm = normalize(Normal) ;
		float diff = max(dot(norm, flashLightDirection), 0.0f);
		vec3 diffuse = flashLight.lightColor * flashLight.diffuseIntensity * diff;
		
		// Calculate specular lightning for flaslight
		vec3 specular = vec3(0.0f);
		if(diff > 0.0f) 
		{
			// Calculate vector between fragment and eye
			vec3 fragToCam = normalize(cameraPosition - FragPos);
			// Calculate halfwayDir for Blinn Phong
			vec3 halfwayDir = normalize(flashLightDirection + fragToCam);
			
			// Calculate angle between normal and halfwayDir
			float flashLightSpecularFactor = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
			
			specular = flashLight.lightColor * flashLight.diffuseIntensity * material.specularIntensity * flashLightSpecularFactor;
		}
		vec3 combinedLight = ambient + (1.0 - shadowFactor) * (diffuse + specular);
		flashLightColor = vec4(combinedLight * flashAttenuation * spotIntensity, 1.0f);
	}
	return flashLightColor;
}


vec4 CalculatePointLight(PointLight pointLight, int shadowIndex){
	// Calculate vector between point light source and current fragment
	vec3 pointLightDistanceVector = pointLight.lightPosition - FragPos;
	
	// Calculate distance between point light source and current fragment
	float distance = length(pointLightDistanceVector);
	
	// Calculate direction vector between point light source and current fragment
	vec3 pointLightDirection = normalize(pointLightDistanceVector);
	
	//Calculate shadow factor
	float shadowFactor = CalculateOmniShadowFactor(pointLight, shadowIndex);
	
	// Calculate attenuation using formula
	float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * distance * distance);
	
	// Calculate ambient color and multiply by attenuation
	vec4 pointLightAmbientColor = vec4(pointLight.lightColor, 1.0f) * pointLight.lightAmbientIntensity;
	
	// Calculate diffuse color and multiply by attenuation
	float pointDiffuseFactor = max(dot(Normal, pointLightDirection), 0.0f);
	vec4 pointLightDiffuseColor = vec4(pointLight.lightColor, 1.0f) * pointLight.lightDiffuseIntensity * pointDiffuseFactor;
	
	// Initialize empty vec4 for specular lightning
	vec4 pointLightSpecularColor = vec4(0.0f);
	
	// Check if specular light is neccesery 
	if (pointDiffuseFactor > 0.0f){
		// Calculate direction we're looking at fragment with
		vec3 fragToCam = normalize(cameraPosition - FragPos);
		
		// Calculate halfwayDir for Blinn Phong
		vec3 halfwayDir = normalize(pointLightDirection + fragToCam);
		
		// Calculate angle between normal and halfwayDir 
		float pointSpecularFactor = pow(max(dot(normalize(Normal), halfwayDir), 0.0), material.shininess);
		if(pointSpecularFactor > 0.0f)
		{
			// Calculate specular color and multiply it by attenuation
			pointLightSpecularColor = vec4(pointLight.lightColor * material.specularIntensity * pointSpecularFactor, 1.0f);
		}
	}
	return(pointLightAmbientColor + (1.0 - shadowFactor)*( pointLightDiffuseColor + pointLightSpecularColor)) * attenuation;
}



void main()
{
	vec4 textureColor = texture(material.textureMap, TextureCoordinates);
	vec4 directionalLightFinalColor = CalculateDirectionalLight();
	vec4 flashLightFinalColor = CalculateFlashLight();
	vec4 pointLightFinalColor = vec4(0.0f);
	
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
        pointLightFinalColor += CalculatePointLight(pointLights[i],i);    

	// Summmarize all light colors together
	vec4 finalLightColor = directionalLightFinalColor + pointLightFinalColor + flashLightFinalColor;
	
	// Multiply the color by light 
	colour = finalLightColor * textureColor;
}