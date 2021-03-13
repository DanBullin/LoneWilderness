#version 440 core
			
layout(location = 0) out vec4 FragColor;

in vec2 texCoord;
in flat int texUnit1;
in flat int texUnit2;
in vec4 tint;

uniform sampler2D[16] u_textures;

layout(std140) uniform Bloom
{
	bool u_horizontal;
	bool u_enableBloom;
};

void main()
{
    float exposure = 1.0;
    const float gamma = 2.2;
    vec3 hdrColor = texture(u_textures[texUnit1], texCoord).rgb;      
    vec3 bloomColor = texture(u_textures[texUnit2], texCoord).rgb;
	
    if(u_enableBloom)
        hdrColor += bloomColor; // additive blending
	
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	
    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / gamma));
	
    FragColor = vec4(result, 1.0);
}