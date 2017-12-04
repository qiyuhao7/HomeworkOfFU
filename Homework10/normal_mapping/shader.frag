#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec3 Tangent;
    vec3 Bitangent;
    mat3 TBN;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;


out vec4 color;

uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D normalMap;

void main()
{   

    vec3 normal = fs_in.Normal;
    mat3 tbn;
	normal = texture(normalMap, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);  

	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normal;
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5f;
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;  

	 vec3 result = (ambient + diffuse + specular) * objectColor;

    //color = texture(texture_diffuse1, TexCoords);
	//color = vec4(0.8f,0.8f,0.8f,1.0f);
	color = vec4(result, 1.0f);
}