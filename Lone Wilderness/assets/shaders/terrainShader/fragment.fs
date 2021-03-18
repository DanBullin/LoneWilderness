#version 450 core
	
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in TES_OUT {
	vec2 TexCoords;
	flat int TexUnit;
	vec4 Tint;
} fs_in;

uniform sampler2D[16] u_diffuseMap;

void main()
{    
    FragColor = texture(u_diffuseMap[fs_in.TexUnit], fs_in.TexCoords) * fs_in.Tint;
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}