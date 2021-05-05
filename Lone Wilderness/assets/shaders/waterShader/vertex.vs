#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 aModel;
layout (location = 9) in int aTexUnit1;
layout (location = 10) in int aTexUnit2;
layout (location = 11) in int aTexUnit3;
layout (location = 12) in vec4 aTint;

// Interface block for the outputs of the vertex shader
out VS_OUT {
	vec4 Tint;
	flat int TexUnit1;
	flat int TexUnit2;
	flat int TexUnit3;
	vec4 ClipSpace;
	vec2 TexCoords;
} vs_out;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
	vec3 u_viewPos;
};

const float tiling = 6.0;

void main()
{
	// Output the tint
	vs_out.Tint = aTint;
	vs_out.ClipSpace = u_projection * u_view * aModel * vec4(aPos, 1.0);
	vs_out.TexUnit1 = aTexUnit1;
	vs_out.TexUnit2 = aTexUnit2;
	vs_out.TexUnit3 = aTexUnit3;
	//vs_out.TexCoords = vec2(aPos.x/2.0 + 0.5, aPos.y/2.0 + 0.5) * tiling;
	vs_out.TexCoords = aTexCoords;
	
    gl_Position = vs_out.ClipSpace;
}