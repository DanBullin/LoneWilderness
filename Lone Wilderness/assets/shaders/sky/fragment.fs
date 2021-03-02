#version 440 core
			
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    	vec3 TexCoords;
	flat int CubemapUnit;
    	vec4 Tint;
} fs_in;

uniform samplerCube[16] u_cubeMap;

void main()
{    
	FragColor = texture(u_cubeMap[fs_in.CubemapUnit], fs_in.TexCoords) * fs_in.Tint;
	BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}