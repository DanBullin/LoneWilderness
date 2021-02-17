#version 440 core
			
out vec4 FragColor;

in VS_OUT {
    	vec3 TexCoords;
		flat int CubemapUnit;
    	vec4 Tint;
} fs_in;

uniform samplerCube[16] u_cubeMap;

void main()
{    
	FragColor = texture(u_cubeMap[fs_in.CubemapUnit], fs_in.TexCoords) * fs_in.Tint;
}