#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in int aCubemapUnit;
layout (location = 6) in vec4 aTint;

// Interface block for the outputs of the vertex shader
out VS_OUT {
	vec3 TexCoords;
	flat int CubemapUnit;
	vec4 Tint;
} vs_out;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
};

void main()
{
    	// Output the texture coordinates
    	vs_out.TexCoords = aPos;
	
	// Output the texture unit
	vs_out.CubemapUnit = aCubemapUnit;
	
	// Output the tint
	vs_out.Tint = aTint;
	
	// Output the position of the model
	vec4 pos = u_projection * mat4(mat3(u_view)) * vec4(aPos, 1.0);
    	gl_Position = pos.xyww;
}