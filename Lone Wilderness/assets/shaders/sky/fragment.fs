#version 440 core
			
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
	flat int TexUnit1;
	flat int TexUnit2;
	flat int TexUnit3;
	flat int TexUnit4;
	flat int CubemapUnit;
    vec4 Tint;
} fs_in;

uniform sampler2D[16] u_diffuseMap;

void main()
{    
	FragColor = mix(texture(u_diffuseMap[fs_in.TexUnit1], fs_in.TexCoords), texture(u_diffuseMap[fs_in.TexUnit2], fs_in.TexCoords), 0.8) * fs_in.Tint;
}