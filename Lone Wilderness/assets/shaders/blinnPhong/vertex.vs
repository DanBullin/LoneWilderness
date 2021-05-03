#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in mat4 aModel;
layout (location = 9) in int aTexUnit1;
layout (location = 10) in int aTexUnit2;
layout (location = 11) in vec4 aTint;
layout (location = 12) in float aShininess;
layout (location = 13) in vec4 aSubTextureUV;

// Interface block for the outputs of the vertex shader
out VS_OUT {
	vec2 TexCoords1;
	vec2 TexCoords2;
	flat int TexUnit1;
	flat int TexUnit2;
	vec4 Tint;
	vec3 ViewPos;
	vec3 Normal;
	vec3 FragPos;
	float Shininess;
} vs_out;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
	vec3 u_viewPos;
};

layout(std140) uniform Clip
{
	vec4 u_plane;
	float u_mode;
};

void main()
{
    // Output the texture coordinates
    vs_out.TexCoords1.x = aSubTextureUV.x + ((aSubTextureUV.z - aSubTextureUV.x) * aTexCoords.x);
    vs_out.TexCoords1.y = aSubTextureUV.y + ((aSubTextureUV.w - aSubTextureUV.y) * aTexCoords.y);
	
	vs_out.TexCoords2 = aTexCoords;
	
	// Output the texture unit
	vs_out.TexUnit1 = aTexUnit1;
	vs_out.TexUnit2 = aTexUnit2;
	
	// Output the tint
	vs_out.Tint = aTint;

	// Output View position
	vs_out.ViewPos = u_viewPos;
	
	vs_out.FragPos = vec3(aModel * vec4(aPos, 1.0));
    vs_out.Normal = aNormal;  
	
	vs_out.Shininess = aShininess;

	gl_ClipDistance[0] = dot(vec4(vs_out.FragPos,1.0), u_plane);
    gl_Position = u_projection * u_view * aModel * vec4(aPos, 1.0);
}