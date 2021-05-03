#version 440 core
			
layout (location = 0) out vec4 FragColor;

in VS_OUT {
	vec4 Tint;
} fs_in;

void main()
{    
    	FragColor = fs_in.Tint;
}