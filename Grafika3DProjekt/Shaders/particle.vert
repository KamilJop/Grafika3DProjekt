#version 330 core
layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 offset; 
uniform vec4 color;
uniform float scale;

void main()
{
    
    TexCoords = aTexCoords;
    ParticleColor = color;

    vec3 CameraRight = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 CameraUp    = vec3(view[0][1], view[1][1], view[2][1]);

    vec3 vertexPosition_world = offset 
                              + CameraRight * aPos.x * scale 
                              + CameraUp * aPos.y * scale;

    gl_Position = projection * view * vec4(vertexPosition_world, 1.0);
}