#version 450 core
	
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in GS_OUT {
	vec3 Normals;
	vec2 TexCoords;
	flat int TexUnit;
	vec4 Tint;
	vec3 ViewPos;
	vec3 FragPos;
} fs_in;

struct DirectionalLight {
	vec4 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
	
struct PointLight {
    vec4 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
	vec4 position;
	vec4 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float cutOff;
	float outerCutOff;
	float constant;
	float linear;
	float quadratic;
};

layout(std140) uniform Settings
{
	bool u_applyFog;
};

layout(std140) uniform Tessellation
{
	int u_tessellationEquation;
	bool u_generateY;
	int u_octaves;
	float u_scale;
	float u_frequency;
	float u_amplitude;
	float u_amplitudeDivisor;
	float u_frequencyMultiplier;
};

layout(std140) uniform DirectionalLights
{
	DirectionalLight dirLight;
};

layout(std140) uniform PointLights
{
	PointLight pointLight[10];
};

layout(std140) uniform SpotLights
{
	SpotLight spotLight[10];
};

uniform sampler2D[16] u_diffuseMap;

const float shininess = 32.0;

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec3 Colour)
{
    vec3 lightDir = normalize(-(light.direction).xyz);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient = (light.ambient).xyz * Colour;
    vec3 diffuse = (light.diffuse).xyz * diff * Colour;
    vec3 specular = (light.specular).xyz * spec * vec3(0.0, 0.0, 0.0);
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Colour)
{
    vec3 lightDir = normalize((light.position).xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	
    // attenuation
    float distance = length((light.position).xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
    // combine results
    vec3 ambient = (light.ambient).xyz * Colour;
    vec3 diffuse = (light.diffuse).xyz * diff * Colour;
    vec3 specular = (light.specular).xyz * spec * vec3(0.0, 0.0, 0.0);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 Colour)
{
    vec3 lightDir = normalize((light.position).xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// attenuation
    float distance = length((light.position).xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
       
    // spotlight intensity
    float theta = dot(lightDir, normalize(-(light.direction).xyz)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = (light.ambient).xyz * Colour;
    vec3 diffuse = (light.diffuse).xyz * diff * Colour;
    vec3 specular = (light.specular).xyz * spec * vec3(0.0, 0.0, 0.0);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{
	vec3 col = vec3(0.0, 0.0, 0.0);
	
	float height = fs_in.FragPos.y / u_scale;

	vec4 green = vec4(0.3, 0.35, 0.15, 0.0);
	vec4 white = vec4(1, 1, 1, 0.0);
	vec4 grey = vec4(0.5, 0.4, 0.5, 0.0);
	vec4 blue = vec4(0, 0.41, 0.58, 0.5);

	if(height > 0.9)
	{
		col = vec3(mix(grey, white, smoothstep(0.9, 1.0, height)).rgb);
	}
	else if(height > 0.5)
	{
		col = vec3(mix(green, grey, smoothstep(0.5, 0.8, height)).rgb);
	}
	else if(height > 0)
	{
		col = vec3(mix(blue, green, smoothstep(0.0, 0.5, height)).rgb);
	}
	
	// properties
    vec3 norm = fs_in.Normals;
    vec3 viewDir = normalize(fs_in.ViewPos - fs_in.FragPos);
    
    vec3 result = vec3(0.0, 0.0, 0.0);
	
	if(dirLight.direction != vec4(0.0, 0.0, 0.0, 0.0))
		result += CalcDirLight(dirLight, norm, viewDir, col);

    for(int i = 0; i < 10; i++)
	{
		if(pointLight[i].constant != 1.0)
			break;
		
		result += CalcPointLight(pointLight[i], norm, fs_in.FragPos, viewDir, col); 	
	}

	for(int i = 0; i < 10; i++)
	{
		if(spotLight[i].constant != 1.0)
			break;
		
		result += CalcSpotLight(spotLight[i], norm, fs_in.FragPos, viewDir, col); 	
	}
	
	float distanceFromCamera = distance(fs_in.ViewPos, fs_in.FragPos);
	float visibility = exp(-pow((distanceFromCamera*0.02), 1.2));
	visibility = clamp(visibility, 0.0, 1.0);
	
	if(u_applyFog == true)
	{
		result = mix(vec3(0.5, 0.5, 0.5), result, visibility);
	}

	// check whether result is higher than some threshold, if so, output as bloom threshold color
    BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	
    FragColor = vec4(result, 1.0) * fs_in.Tint;
}