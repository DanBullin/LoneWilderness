#version 440 core
			
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
    	vec3 TexCoords;
	flat int CubemapUnit;
    	vec4 Tint;
} fs_in;

uniform samplerCube[16] u_cubeMap;

const float LOWERLIMIT = 0.0;
const float UPPERLIMIT = 1.0;

void main()
{
	float factor = (fs_in.TexCoords.y - LOWERLIMIT) / (UPPERLIMIT - LOWERLIMIT);
	factor = clamp(factor, 0.0, 1.0);
	vec4 finalColour = texture(u_cubeMap[fs_in.CubemapUnit], fs_in.TexCoords);
	
	FragColor = mix(vec4(0.5, 0.5, 0.5, 1.0), finalColour, factor);
	BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}