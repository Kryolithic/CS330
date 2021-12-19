#version 460 core
out vec4 FragColor;

//structure to hold material types
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

//structure to hold lighting values
struct DirectLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	
	float constant;
	float linear;
	float quadratic;
	float ambientStr;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

//uniforms
uniform vec3 viewPosition;
uniform Material material;
uniform DirectLight directLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

//augment slot for textures
uniform sampler2D texAugment;

// Texture scaler uniform
uniform vec2 uvScale;

//functions
vec3 CalcDirect(DirectLight light, vec3 normal, vec3 viewDirection);
vec3 CalcPoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);
vec3 CalcSpot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection);

void main()
{
    /*Phong lighting model calculations to generate ambient, diffuse, and specular components*/

	vec3 norm = normalize(Normal); // Normalize vectors to 1 unit
	vec3 viewDirection = normalize(viewPosition - FragPos);


	vec3 result = CalcDirect(directLight, norm, viewDirection);
	result += CalcPoint(pointLight, norm, FragPos, viewDirection);
	//result += CalcSpot(spotLight, norm, FragPos, viewDirection);

	FragColor = vec4(result, 1.0);
}
//==================================================

//Calculate Diffuse lighting*/
vec3 CalcDirect(DirectLight light, vec3 normal, vec3 viewDirection)
{
                  
        vec3 lightDir = normalize(-light.direction);                                        // Calculate distance (light direction) between light source and fragments/pixels on cube
        float diff = max(dot(normal, lightDir), 0.0);                                       // Calculate diffuse impact by generating dot product of normal and light
                                       
        vec3 reflectDir = reflect(-lightDir, normal);                                         // Calculate reflection vector
        float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

        // Texture holds the color to be used for all three components
        vec4 textureColor = mix(texture(material.diffuse, TexCoord * uvScale), texture(texAugment, TexCoord * uvScale), 0.99);

        // Calculate phong result
        vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

        return (phong);                                                        // Send lighting results to GPU
}

//Calculate Point Lighting
vec3 CalcPoint(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{

	vec3 lightDir = normalize(light.position - fragPos);

	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDirection = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 ambient = light.ambient * light.ambientStr * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
        vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// Texture holds the color to be used for all three components
        vec4 textureColor = mix(texture(material.diffuse, TexCoord * uvScale), texture(texAugment, TexCoord * uvScale), 0.99);

        // Calculate phong result
        vec3 phong = (ambient + diffuse + specular) * textureColor.xyz;

        return (phong);
}

//Calculate Spotlighting
vec3 CalcSpot(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDirection)
{
	vec3 lightDir = normalize(light.position - fragPos);
    
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
    
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    
	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction)); 
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
    
	return (ambient + diffuse + specular);
}