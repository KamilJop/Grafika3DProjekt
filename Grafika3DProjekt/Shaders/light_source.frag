#version 330

out vec4 colour;

uniform vec3 bulbColor; 

void main()
{
    colour = vec4(bulbColor, 1.0);
}