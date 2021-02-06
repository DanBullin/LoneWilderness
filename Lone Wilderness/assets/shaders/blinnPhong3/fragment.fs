#version 440 core
			
out vec4 FragColor;

in VS_OUT {
    vec2 TexCoords;
	flat int TexUnit1;
	flat int TexUnit2;
	flat int TexUnit3;
	flat int TexUnit4;
    vec4 Tint;
} fs_in;

uniform sampler2D[16] u_diffuseMap;

void main()
{
	float t = 4.0;
	int u = int(t);    
	FragColor = texture(u_diffuseMap[u], fs_in.TexCoords) * fs_in.Tint;
}