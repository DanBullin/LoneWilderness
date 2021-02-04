#version 440 core
			
layout(location = 0) out vec4 colour;

in vec2 texCoord;
in flat int texUnit;
in vec4 tint;

uniform sampler2D[16] u_diffuseMap;

void main()
{
	 //colour = mix(texture(u_diffuseMap[texUnit], texCoord), texture(u_diffuseMap[texUnit + 1], texCoord), 0.2) * tint;
	 colour = vec4(vec3(1.0 - texture(u_diffuseMap[texUnit], texCoord)), 1.0) * tint;
}