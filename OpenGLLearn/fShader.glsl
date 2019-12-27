#version 330 core
out vec4 FragColor;

struct Material {
    //sampler2D ambientTex; // Map_Ka
    sampler2D diffuseTex; // Map_Kd
    sampler2D specularTex; // Map_Ks
    sampler2D bumpTex; // Map_Bump
	
	bool diffuseTexValid;
	bool specularTexValid;
	bool bumpTexValid;

    vec3 ambient; // Ka
    vec3 diffuse; // Kd
    vec3 specular; // Ks

    float shininess; // Ns
}; 

struct Light {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

in vec3 CamPos;

uniform float duration = 0;
uniform bool exploding = false;

#define MaxLightCount 50
layout (std140) uniform Lights {
	Light lights[MaxLightCount];
};

uniform Material material;

vec3 calcLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir) {

    vec3 lightDir = normalize(light.position - fragPos);

    // 漫反射着色
    float diff = max(dot(normal, lightDir), 0.0);

    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // 衰减
    float distance    = length(light.position - fragPos);
    float attenuation = 1 / (light.constant + light.linear * distance + 
                 light.quadratic * (distance * distance));

    // 合并结果
	// 环境光
	vec3 mAmbi = material.ambient;
	//if (material.diffuseTexValid) 
		mAmbi = vec3(texture(material.diffuseTex, TexCoord));
    vec3 ambient  = light.ambient  * mAmbi;
	
	// 漫反射
	vec3 mDiff = material.diffuse;
	//if (material.diffuseTexValid) 
		mDiff = vec3(texture(material.diffuseTex, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * mDiff;

	// 镜面光
	vec3 mSpec = material.specular;
	//if (material.specularTexValid) 
		mSpec = vec3(texture(material.specularTex, TexCoord));
    vec3 specular = light.specular * spec * mSpec;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
	//return vec3(distance/1000);
	//return vec3(light.constant, light.linear, light.quadratic);

	/*
    // 漫反射着色
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);

    // 镜面光着色
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // 合并
	// 环境光
    //vec3 ambient  = light.ambient * material.ambient;
	vec3 mAmbi = material.ambient;
	//if (material.diffuseTexValid) 
		mAmbi = vec3(texture(material.diffuseTex, TexCoord));
    vec3 ambient  = light.ambient * mAmbi; 

	// 漫反射
	vec3 mDiff = material.diffuse;
	//if (material.diffuseTexValid) 
		mDiff = vec3(texture(material.diffuseTex, TexCoord));
    vec3 diffuse  = light.diffuse  * diff * mDiff;
	
	vec3 mSpec = material.specular;
	//if (material.specularTexValid) 
		mSpec = vec3(texture(material.specularTex, TexCoord));
	// 镜面光
    vec3 specular = light.specular * spec * mSpec;

    return (ambient + diffuse + specular);
	*/
}

void main() {

    // 属性
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(CamPos - FragPos);

	vec3 result = vec3(0);

    for(int i = 0; i < 1; i++)
        result += calcLight(lights[i], norm, FragPos, viewDir);   
		
	//FragColor = vec4(lights[0].ambient,1.0);
    FragColor = vec4(result, 1-sin(duration));
}
