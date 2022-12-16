#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec4 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 ourColor;

void main()
{
	TexCoord = aTexCoords;   
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	color = ourColor;
}
/*
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = projection * view * aInstanceMatrix * vec4(aPos, 1.0f); 
}*/
/*

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstancedMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(aInstancedMatrix * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(aInstancedMatrix))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * aInstancedMatrix * vec4(FragPos, 1.0);
}*/
