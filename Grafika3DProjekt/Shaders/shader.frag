/**
 * @file lighting.frag
 * @brief Main fragment shader for lighting, shadows, normal mapping, and parallax mapping.
 *
 * Handles:
 * - Directional light with PCF shadows
 * - Point lights with omni shadow maps
 * - Flashlight cone lighting
 * - Normal mapping (TBN)
 * - Parallax mapping
 * - HDR bright-pass output
 */

#version 330
#define NR_POINT_LIGHTS 15

// Inputs from vertex shader
in vec4 vCol;
in vec3 Normal;
in vec3 FragPos;
in vec2 TextureCoordinates;
in vec4 DirectionalLightSpacePosition;
in vec4 FlashLightSpacePosition;
in mat3 TBN;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// Outputs
layout (location = 0) out vec4 colour;
layout (location = 1) out vec4 BrightColor;

// ----------------------------
// Structs
// ----------------------------
struct DirectionalLight {
    vec3 lightColor;
    vec3 lightDirection;
    float lightAmbientIntensity;
    float lightDiffuseIntensity;
};

struct PointLight {
    vec3 lightColor;
    vec3 lightPosition;
    float lightAmbientIntensity;
    float lightDiffuseIntensity;
    float constant;
    float linear;
    float quadratic;
    int castsShadows;
};

struct OmniShadowMap {
    samplerCube shadowMap;
    float farPlane;
};

struct FlashLight {
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

struct Material {
    float specularIntensity;
    float shininess;
    sampler2D textureMap;
    sampler2D normalMap;
    sampler2D heightMap;
};

// ----------------------------
// Uniforms
// ----------------------------
uniform DirectionalLight directionalLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform FlashLight flashLight;
uniform Material material;
uniform sampler2D directionalShadowMap;
uniform sampler2D flashShadowMap;
uniform OmniShadowMap omniShadowMaps[NR_POINT_LIGHTS];
uniform int isSelected;
uniform vec3 cameraPosition;

// Sample offsets for omni shadow PCF
vec3 sampleOffsetDirections[20] = vec3[](
    vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1),
    vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
    vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
    vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
    vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

/**
 * @brief Computes PCF shadow factor for directional light.
 */
float CalculateDirectionalShadowFactor()
{
    vec3 projCoords = DirectionalLightSpacePosition.xyz / DirectionalLightSpacePosition.w;
    projCoords = projCoords * 0.5 + 0.5;

    float current = projCoords.z;
    float bias = 0.001;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);

    for(int x = -1; x <= 1; x++)
        for(int y = -1; y <= 1; y++)
            shadow += current - bias > texture(directionalShadowMap, projCoords.xy + vec2(x,y)*texelSize).r ? 1.0 : 0.0;

    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

/**
 * @brief Computes PCF shadow factor for flashlight.
 */
float CalculateFlashLightShadowFactor()
{
    vec3 projCoords = FlashLightSpacePosition.xyz / FlashLightSpacePosition.w;
    projCoords = projCoords * 0.5 + 0.5;

    float current = projCoords.z;
    float bias = 0.00001;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(flashShadowMap, 0);

    for(int x = -1; x <= 1; x++)
        for(int y = -1; y <= 1; y++)
            shadow += current - bias > texture(flashShadowMap, projCoords.xy + vec2(x,y)*texelSize).r ? 1.0 : 0.0;

    shadow /= 9.0;

    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

/**
 * @brief Computes omni-directional shadow factor for point lights.
 */
float CalculateOmniShadowFactor(PointLight pLight, int shadowIndex)
{
    vec3 fragToLight = FragPos - pLight.lightPosition;
    float current = length(fragToLight);

    float bias = 0.01;
    float shadow = 0.0;
    int samples = 4;

    float viewDistance = length(cameraPosition - FragPos);
    float diskRadius = (1.0 + viewDistance / omniShadowMaps[shadowIndex].farPlane) / 25.0;

    for(int i = 0; i < samples; i++)
    {
        float closest = texture(omniShadowMaps[shadowIndex].shadowMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closest *= omniShadowMaps[shadowIndex].farPlane;

        if(current - bias > closest)
            shadow += 1.0;
    }

    return shadow / float(samples);
}

/**
 * @brief Computes directional light contribution.
 */
vec4 CalculateDirectionalLight(vec3 worldNormal)
{
    float shadow = CalculateDirectionalShadowFactor();

    vec4 ambient = vec4(directionalLight.lightColor, 1.0) * directionalLight.lightAmbientIntensity;

    float diff = max(dot(worldNormal, normalize(-directionalLight.lightDirection)), 0.0);
    vec4 diffuse = vec4(directionalLight.lightColor, 1.0) * directionalLight.lightDiffuseIntensity * diff;

    vec4 specular = vec4(0.0);
    if(diff > 0.0)
    {
        vec3 viewDir = normalize(cameraPosition - FragPos);
        vec3 halfway = normalize(normalize(-directionalLight.lightDirection) + viewDir);
        float spec = pow(max(dot(worldNormal, halfway), 0.0), material.shininess);
        specular = vec4(directionalLight.lightColor * directionalLight.lightDiffuseIntensity * material.specularIntensity * spec, 1.0);
    }

    return ambient + (1.0 - shadow) * (diffuse + specular);
}

/**
 * @brief Computes flashlight contribution.
 */
vec4 CalculateFlashLight(vec3 worldNormal)
{
    float shadow = CalculateFlashLightShadowFactor();

    vec3 toFrag = flashLight.lightPosition - FragPos;
    float dist = length(toFrag);
    vec3 dir = normalize(toFrag);

    float theta = dot(dir, normalize(-flashLight.lightDirection));
    if(theta <= flashLight.outerCutOff)
        return vec4(0.0);

    float attenuation = 1.0 / (flashLight.constant + flashLight.linear * dist + flashLight.quadratic * dist * dist);

    float epsilon = flashLight.cutOff - flashLight.outerCutOff;
    float intensity = clamp((theta - flashLight.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = flashLight.lightColor * flashLight.ambientIntensity;

    float diff = max(dot(worldNormal, dir), 0.0);
    vec3 diffuse = flashLight.lightColor * flashLight.diffuseIntensity * diff;

    vec3 specular = vec3(0.0);
    if(diff > 0.0)
    {
        vec3 viewDir = normalize(cameraPosition - FragPos);
        vec3 halfway = normalize(dir + viewDir);
        float spec = pow(max(dot(worldNormal, halfway), 0.0), material.shininess);
        specular = flashLight.lightColor * flashLight.diffuseIntensity * material.specularIntensity * spec;
    }

    vec3 result = ambient + (1.0 - shadow) * (diffuse + specular);
    return vec4(result * attenuation * intensity, 1.0);
}

/**
 * @brief Computes point light contribution.
 */
vec4 CalculatePointLight(PointLight p, int shadowIndex, vec3 worldNormal)
{
    float dist = length(p.lightPosition - FragPos);
    if(dist > 15.0)
        return vec4(0.0);

    vec3 toFrag = p.lightPosition - FragPos;
    float distance = length(toFrag);
    vec3 dir = normalize(toFrag);

    float shadow = p.castsShadows == 1 ? CalculateOmniShadowFactor(p, shadowIndex) : 0.0;

    float attenuation = 1.0 / (p.constant + p.linear * distance + p.quadratic * distance * distance);

    vec4 ambient = vec4(p.lightColor, 1.0) * p.lightAmbientIntensity;

    float diff = max(dot(worldNormal, dir), 0.0);
    vec4 diffuse = vec4(p.lightColor, 1.0) * p.lightDiffuseIntensity * diff;

    vec4 specular = vec4(0.0);
    if(diff > 0.0)
    {
        vec3 viewDir = normalize(cameraPosition - FragPos);
        vec3 halfway = normalize(dir + viewDir);
        float spec = pow(max(dot(worldNormal, halfway), 0.0), material.shininess);
        specular = vec4(p.lightColor * material.specularIntensity * spec, 1.0);
    }

    return (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation;
}

/**
 * @brief Performs parallax mapping on texture coordinates.
 */
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    float minLayers = 8.0;
    float maxLayers = 32.0;
    float numLayers = mix(maxLayers, minLayers, max(dot(vec3(0,0,1), viewDir), 0.0));

    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;

    float heightScale = 0.026;
    vec2 p = viewDir.xy * heightScale;
    vec2 delta = p / numLayers;

    vec2 current = texCoords;
    float depth = texture(material.heightMap, current).r;

    while(currentLayerDepth < depth)
    {
        current -= delta;
        depth = texture(material.heightMap, current).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prev = current + delta;

    float after = depth - currentLayerDepth;
    float before = texture(material.heightMap, prev).r - currentLayerDepth + layerDepth;

    float weight = after / (after - before);
    return prev * weight + current * (1.0 - weight);
}

/**
 * @brief Main fragment shader entry point.
 */
void main()
{
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 texCoords = ParallaxMapping(TextureCoordinates, viewDir);

    vec3 normal = texture(material.normalMap, texCoords).rgb;
    normal = normalize(TBN * (normal * 2.0 - 1.0));

    vec4 texColor = texture(material.textureMap, texCoords);

    vec4 dirLight = CalculateDirectionalLight(normal);
    vec4 flash = CalculateFlashLight(normal);

    vec4 pointLightsColor = vec4(0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        pointLightsColor += CalculatePointLight(pointLights[i], i, normal);

    vec4 finalLight = dirLight + pointLightsColor + flash;

    colour = finalLight * texColor;

    if(isSelected == 1)
        colour.rgb += vec3(0.3, 0.3, 0.0);

    float brightness = dot(colour.rgb, vec3(0.2126, 0.7152, 0.0722));
    BrightColor = brightness >= 1.0 ? colour : vec4(0.0, 0.0, 0.0, 1.0);
}
