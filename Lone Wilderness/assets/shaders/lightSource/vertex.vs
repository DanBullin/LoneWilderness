#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 aModel;
layout (location = 9) in int aTexUnit;
layout (location = 10) in vec4 aTint;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
};

out vec4 Tint;

void main()
{
	Tint = aTint;
    gl_Position = u_projection * u_view * aModel * vec4(aPos, 1.0);
}