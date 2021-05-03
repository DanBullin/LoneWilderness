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
layout (location = 13) in float aShininess;
layout (location = 14) in vec4 aSubTextureUV;

struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
};

// Interface block for the outputs of the vertex shader
out VS_OUT {
	vec2 TexCoords1;
	vec2 TexCoords2;
	flat int TexUnit1;
	flat int TexUnit2;
	flat int TexUnit3;
	vec4 Tint;
	vec3 ViewPos;
	vec3 TangentViewPos;
	vec3 Normal;
	vec3 FragPos;
	vec3 TangentFragPos;
	float Shininess;
	vec3 PointLightsPositions[10];
} vs_out;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
	vec3 u_viewPos;
};

layout(std140) uniform PointLights
{
	PointLight pointLight[10];
};

void main()
{
	// Calculate the normal matrix
    mat3 normalMatrix = transpose(inverse(mat3(aModel)));
	
	// Calculate the TBN matrix which transforms anything to tangent space
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
	
	// Use the Gram-Schmidt process to re-orthogonalize the TBN vectors
    T = normalize(T - dot(T, N) * N);
    vec3 B = normalize(normalMatrix * aBitangent);
 
    mat3 TBN = transpose(mat3(T, B, N));
	
    // Output the texture coordinates
    vs_out.TexCoords1.x = aSubTextureUV.x + ((aSubTextureUV.z - aSubTextureUV.x) * aTexCoords.x);
    vs_out.TexCoords1.y = aSubTextureUV.y + ((aSubTextureUV.w - aSubTextureUV.y) * aTexCoords.y);
	
	vs_out.TexCoords2 = aTexCoords;
	
	// Output the texture unit
	vs_out.TexUnit1 = aTexUnit1;
	vs_out.TexUnit2 = aTexUnit2;
	vs_out.TexUnit3 = aTexUnit3;
	
	// Output the tint
	vs_out.Tint = aTint;

	// Output View position
	vs_out.ViewPos = u_viewPos;
	
	vs_out.FragPos = vec3(aModel * vec4(aPos, 1.0));
    vs_out.Normal = normalMatrix * aNormal;  
	vs_out.TangentViewPos = TBN * vs_out.ViewPos;
    vs_out.TangentFragPos = TBN * vs_out.FragPos;
	
	vs_out.Shininess = aShininess;
	
	for(int i = 0; i < 10; i++)
	{
		if(pointLight[i].constant != 1.0)
			break;
		
		vs_out.PointLightsPositions[i] = vec3(TBN * vec3(pointLight[i].position));
		//vec3 tangentPos = TBN * pointLight[i].position.xyz;
		//vs_out.PointLights[i].position = vec4(0.0, 0.0, 0.0, 0.0); 	
	}

    gl_Position = u_projection * u_view * aModel * vec4(aPos, 1.0);
}