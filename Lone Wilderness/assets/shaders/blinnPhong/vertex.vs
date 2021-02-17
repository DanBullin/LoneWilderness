#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 aModel;
layout (location = 9) in int aTexUnit;
layout (location = 10) in vec4 aTint;
layout (location = 11) in vec4 aSubTextureUV;

// Interface block for the outputs of the vertex shader
out VS_OUT {
	vec2 TexCoords;
	flat int TexUnit;
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
    vs_out.TexCoords.x = aSubTextureUV.x + ((aSubTextureUV.z - aSubTextureUV.x) * aTexCoords.x);
    vs_out.TexCoords.y = aSubTextureUV.y + ((aSubTextureUV.w - aSubTextureUV.y) * aTexCoords.y);
	
	// Output the texture unit
	vs_out.TexUnit = aTexUnit;
	
	// Output the tint
	vs_out.Tint = aTint;

    gl_Position = u_projection * u_view * aModel * vec4(aPos, 1.0);
}