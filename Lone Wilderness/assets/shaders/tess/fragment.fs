#version 330 core
out vec4 FragColor;

in vec3 normalES;
in vec3 posES;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
};                                                                        

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
}; 

//uniform sampler2D texture1;
uniform DirLight dirLight;
uniform Material mat ;
uniform vec3 viewPos ;


void main()
{   
    vec3 col = vec3(0.6,0.1,0.2) ;
  
     vec3 viewDir = normalize(viewPos - posES);
	 vec3 norm = normalize(normalES) ;
	 vec3 ambient = dirLight.ambient * mat.ambient;     
     vec3 lightDir = normalize(-dirLight.direction);
    // diffuse shading
    float diff = max(dot(norm, dirLight.direction), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-dirLight.direction, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
    // combine results
   
    vec3 diffuse  = dirLight.diffuse  * (diff * mat.diffuse);
    vec3 specular = dirLight.specular * (spec * mat.specular);
    FragColor = vec4((ambient + diffuse + specular),1.0f);
//  FragColor = vec4((ambient+diffuse),1.0f) ;
 //FragColor =  texture(texture1, TexCoords);
//FragColor = vec4(normalES,1.0f) ;

	
}

