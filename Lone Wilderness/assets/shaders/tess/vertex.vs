#version 450 core
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

out vec3 posVS; 
out vec3 normVS;

void main()
{
	// Sent the vertex positions and normals in world space to the TCS
    normVS = normalize((aModel * vec4(aNormals, 0.0)).xyz); 
	posVS = (aModel * vec4(aPos, 1.0)).xyz;
}