#version 440 core
			
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT {
	vec4 Tint;
} fs_in;

void main()
{    
    	FragColor = fs_in.Tint;
	BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}