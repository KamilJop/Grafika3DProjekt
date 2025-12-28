#version 330 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor; // <-- KLUCZOWE DLA BLOOM

in vec2 TexCoords;
in vec4 ParticleColor;
uniform sampler2D sprite;

void main()
{
    vec4 texColor = texture(sprite, TexCoords) * ParticleColor;
    if(texColor.a < 0.1) discard;

    FragColor = texColor;
    
    BrightColor = texColor; 
}