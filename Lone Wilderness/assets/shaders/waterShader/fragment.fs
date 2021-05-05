#version 440 core
			
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
	vec4 Tint;
	flat int TexUnit1;
	flat int TexUnit2;
	flat int TexUnit3;
	vec4 ClipSpace;
	vec2 TexCoords;
} fs_in;

layout(std140) uniform Water
{
	float u_moveFactor;
};

uniform sampler2D[16] u_diffuseMap;

const float waveStrength = 0.02;

void main()
{   
	vec2 NDC = (fs_in.ClipSpace.xy/fs_in.ClipSpace.w) / 2.0 + 0.5;
	vec2 refractTexCoords = vec2(NDC.x, NDC.y);
	vec2 reflecTexCoords = vec2(NDC.x, -NDC.y);
	
	vec2 distortion1 = (texture(u_diffuseMap[fs_in.TexUnit3], vec2(fs_in.TexCoords.x + u_moveFactor, fs_in.TexCoords.y)).rg * 2.0 - 1.0) * waveStrength;
	vec2 distortion2 = (texture(u_diffuseMap[fs_in.TexUnit3], vec2(-fs_in.TexCoords.x + u_moveFactor, fs_in.TexCoords.y + u_moveFactor)).rg * 2.0 - 1.0) * waveStrength;
	vec2 totalDistortion = distortion1 + distortion2;
	
	refractTexCoords += totalDistortion;
	reflecTexCoords += totalDistortion;
	
	refractTexCoords = clamp(refractTexCoords, 0.001, 0.999);
	reflecTexCoords.x = clamp(reflecTexCoords.x, 0.001, 0.999);
	reflecTexCoords.y = clamp(reflecTexCoords.y, -0.999, -0.001);
	
	vec4 reflectColour = texture(u_diffuseMap[fs_in.TexUnit1], reflecTexCoords);
	vec4 refractColour = texture(u_diffuseMap[fs_in.TexUnit2], refractTexCoords);
		
    FragColor = mix(reflectColour, refractColour, 0.5);
    FragColor = mix(FragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
	BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}