#version 330 core
// This is a sample fragment shader.
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight{
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight{
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in float sampleExtraOutput;
in vec3 vec;
in float s;
in float l;
in vec3 FragPos;
in vec3 norm;

uniform vec3 color;
//uniform vec3 lightPos;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointlight;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.position);
    float diff = max(dot(normal,lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * material.diffuse;
    vec3 diffuse = light.diffuse * diff * material.diffuse;
    vec3 specular = light.specular * spec * material.specular;
    ambient *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
    /*vec3 lightColor = vec3(1.0);
    vec3 lightPos = light.position;
    vec3 viewPos = vec3(0,0,20);
    // For ambient
    vec3 ambient = light.ambient* material.ambient;
    // For diffuse
    vec3 norm = normalize(vec);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm,lightDir), 0.0f);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    // For specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular* (spec * material.specular);
    vec3 result = (ambient + diffuse + specular) * color;*/
    
    vec3 viewPos = vec3(0,0,20);
    vec3 viewDir = normalize(viewPos - FragPos);
    // Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // Point lights
    result += CalcPointLight(pointlight,norm,FragPos, viewDir);
    
    if(s==0){
        fragColor = vec4(0.5f*normalize(vec)+0.5f, sampleExtraOutput);
    }
    else if(l==1){
        // This is for the light sphere
        fragColor=vec4(color, sampleExtraOutput);
    }
    else{
        fragColor = vec4(result*color, sampleExtraOutput);
    }
    //fragColor = vec4(color,sampleExtraOutput);
}
