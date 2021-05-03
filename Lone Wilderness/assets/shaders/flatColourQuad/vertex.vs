#version 440 core
layout(location = 0) in vec4 a_vertexPosition;
layout(location = 1) in vec2 a_texCoord;
layout(location = 2) in int a_texUnit;
layout(location = 3) in vec4 a_tint;

// Interface block for the outputs of the vertex shader
out VS_OUT {
	vec4 Tint;
} vs_out;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
	vec3 u_viewPos;
};

void main()
{
	// Output the tint
	vs_out.Tint = a_tint;

    gl_Position =  u_projection * u_view * a_vertexPosition;
}