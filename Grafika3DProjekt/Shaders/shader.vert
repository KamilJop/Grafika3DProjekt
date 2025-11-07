#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec4 vCol;
out vec3 Normal;
out vec3 FragPos;


uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;





void main()
{
	gl_Position =  projection * view * model * vec4(aPos, 1.0);
	FragPos = (model * vec4(aPos, 1.0)).xyz;
	Normal = mat3(transpose(inverse(model))) * aNormal;
	vCol = vec4(clamp(aPos, 0.0f, 1.0f), 1.0f);
}