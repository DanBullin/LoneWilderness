#version 440 core
			
layout(location = 0) out vec4 colour;

in vec2 texCoord;
in flat int texUnit;
in vec4 tint;

uniform sampler2D[16] u_diffuseMap;

void main()
{
	 colour = texture(u_diffuseMap[texUnit], texCoord) * tint;
}