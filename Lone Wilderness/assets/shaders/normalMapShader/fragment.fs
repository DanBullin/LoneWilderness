#version 440 core
	
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

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

in VS_OUT {
	vec2 TexCoords1;
	vec2 TexCoords2;
	flat int TexUnit1;
	flat int TexUnit2;
	flat int TexUnit3;
	vec4 Tint;
	vec3 ViewPos;
	vec3 TangentViewPos;
	vec3 Normal;
	vec3 FragPos;
	vec3 TangentFragPos;
	float Shininess;
	vec3 PointLightsPositions[10];
} fs_in;

uniform sampler2D[16] u_diffuseMap;

layout(std140) uniform Camera
{
	mat4 u_view;
	mat4 u_projection;
	vec3 u_viewPos;
};

layout(std140) uniform Settings
{
	bool u_applyFog;
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

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-(light.direction).xyz);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fs_in.Shininess);
    // combine results
    vec3 ambient = (light.ambient).xyz * vec3(texture(u_diffuseMap[fs_in.TexUnit3], fs_in.TexCoords1));
    vec3 diffuse = (light.diffuse).xyz * diff * vec3(texture(u_diffuseMap[fs_in.TexUnit3], fs_in.TexCoords1));
    vec3 specular = (light.specular).xyz * spec * vec3(texture(u_diffuseMap[fs_in.TexUnit2], fs_in.TexCoords2));
    return (ambient + diffuse + specular);
}

// Calculates the color when using a point light.
vec3 CalcPointLight(int index, PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize((fs_in.PointLightsPositions[index]).xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fs_in.Shininess);
	
    // attenuation
    float distance = length((fs_in.PointLightsPositions[index]).xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
    // combine results
    vec3 ambient = (light.ambient).xyz * vec3(texture(u_diffuseMap[fs_in.TexUnit1], fs_in.TexCoords1));
    vec3 diffuse = (light.diffuse).xyz * diff * vec3(texture(u_diffuseMap[fs_in.TexUnit1], fs_in.TexCoords1));
    vec3 specular = (light.specular).xyz * spec * vec3(texture(u_diffuseMap[fs_in.TexUnit2], fs_in.TexCoords2));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// Calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize((light.position).xyz - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), fs_in.Shininess);
	// attenuation
    float distance = length((light.position).xyz - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
       
    // spotlight intensity
    float theta = dot(lightDir, normalize(-(light.direction).xyz)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = (light.ambient).xyz * vec3(texture(u_diffuseMap[fs_in.TexUnit3], fs_in.TexCoords1));
    vec3 diffuse = (light.diffuse).xyz * diff * vec3(texture(u_diffuseMap[fs_in.TexUnit3], fs_in.TexCoords1));
    vec3 specular = (light.specular).xyz * spec * vec3(texture(u_diffuseMap[fs_in.TexUnit2], fs_in.TexCoords2));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

void main()
{    
	// properties
    vec3 norm = normalize(2 * texture(u_diffuseMap[fs_in.TexUnit3], fs_in.TexCoords2).rgb - 1);
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    
    vec3 result = vec3(0.0, 0.0, 0.0);
	
    for(int i = 0; i < 10; i++)
	{
		if(pointLight[i].constant != 1.0)
			break;
		
		result += CalcPointLight(i, pointLight[i], norm, fs_in.TangentFragPos, viewDir); 	
	}
	
	float distanceFromCamera = distance(fs_in.ViewPos, fs_in.FragPos);
	float visibility = exp(-pow((distanceFromCamera*0.08), 1.2));
	visibility = clamp(visibility, 0.0, 1.0);
	
	if(u_applyFog == true)
	{
		result = mix(vec3(0.0, 0.0, 0.0), result, visibility);
	}
	
	// check whether result is higher than some threshold, if so, output as bloom threshold color
    float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	//brightness = mix(vec4(0.0, 0.0, 0.0, 1.0), brightness, visibility);
    if(brightness > 1.0)
        BrightColor = vec4(result, 1.0) * fs_in.Tint;
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	
	FragColor = vec4(result, 1.0) * fs_in.Tint;
}