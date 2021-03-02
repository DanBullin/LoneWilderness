#version 440 core
layout(location = 0) in vec4 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in int a_texUnit;
layout(location = 3) in vec4 a_tint;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
};

out vec2 texCoord;
out flat int texUnit;
out vec4 tint;

void main()
{
	texCoord = vec2(a_texCoord);
	texUnit = a_texUnit;
	tint = a_tint;
	gl_Position =  u_projection * u_view * a_vertexPosition;
}