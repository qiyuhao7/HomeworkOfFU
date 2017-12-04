#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 delta;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 weight;

void main()
{
    vec4 positionNew = vec4(position, 1.0f);
	positionNew =  positionNew - weight*vec4(delta,1.0);
    gl_Position = projection * view * model * positionNew;
	//FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverse(model))) * normal;  
    TexCoords = texCoords;
}