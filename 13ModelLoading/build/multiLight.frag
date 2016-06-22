#version 330 core
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    float kConstant;
    float kLinear;
    float kQuadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;
    
    float kConstant;
    float kLinear;
    float kQuadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NUM_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


out vec4 color;

uniform vec3 cameraPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NUM_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;


// Function prototypes
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );
vec3 CalcPointLight( PointLight light, vec3 normal,  vec3 fragPos, vec3 viewDir );
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

void main()
{
    // properties
    vec3 norm = normalize( Normal );
    vec3 viewDir = normalize( cameraPos - FragPos );

    //==================================================
    // Our lighting is set up in 3 phases: directional, point lights and optional flashlight.
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculate colors and sum them up for
    // this fragments's final color;
    //==================================================
    // Phase 1: Directional Light
    vec3 result = CalcDirLight( dirLight, norm, viewDir );

    // Phase 2: Point Lights
    for( int i = 0; i < NUM_POINT_LIGHTS; i++ )
    {
	result += CalcPointLight( pointLights[i], norm, FragPos, viewDir );
    }

    // Phase 3: Spot Light
    result += CalcSpotLight( spotLight, norm, FragPos, viewDir );

    color = vec4( result, 1.0f );
}

// Calculates the color when using a directional light
vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )
{
    vec3 lightDir = normalize( -light.direction );

    // Diffuse shading
    float diff  = max( dot(normal, lightDir), 0.0 );
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max(dot(viewDir, reflectDir), 0.0), material.shininess );

    // Combine resulets
    vec3 ambient = light.ambient * vec3( texture(material.diffuse, TexCoords) );
    vec3 diffuse = light.diffuse  * diff * vec3( texture(material.diffuse, TexCoords) );
    vec3 specular = light.specular * spec * vec3( texture(material.specular, TexCoords) );

    return ( ambient + diffuse + specular );
}


// Calculates the color when using a point light
vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );
    // Diffuse shading
    float diff = max( dot(lightDir, normal), 0.0 );
    // Specular shading
    vec3 reflectDir = reflect( -lightDir, normal );
    float spec = pow( max(dot(viewDir, reflectDir), 0.0), material.shininess );

    // Attenuation
    float distances = length( light.position - fragPos );
    float attenuation = 1.0f / (light.kConstant +
				light.kLinear * distances +
				light.kQuadratic * (distances * distances));
    // Combine results
    vec3 ambient = light.ambient * vec3( texture(material.diffuse, TexCoords) );
    vec3 diffuse = light.diffuse * diff * vec3( texture(material.diffuse, TexCoords) );
    vec3 specular = light.specular * spec * vec3( texture(material.specular, TexCoords) );

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ( ambient + diffuse + specular );


}

// Calculates the color when using a spot light
vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )
{
    vec3 lightDir = normalize( light.position - fragPos );
    float theta = max( dot(lightDir, -light.direction), 0.0 );
    if( theta > light.outerCutOff )
    {
	// intensity
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0) * 3.0f;
	// diffuse
	float diff = max( dot(lightDir, normal), 0.0 );
	// specular
	vec3 reflectDir = reflect( -lightDir, normal );
	float spec = pow( max(dot(reflectDir, viewDir), 0.0), material.shininess );
	// attenuation
	float distances = length( light.position - fragPos );
	float attenuation = 1.0f / (light.kConstant +
				    light.kLinear * distances +
				    light.kQuadratic * (distances * distances));
	// Combine results
	vec3 diffuse = light.diffuse * diff * vec3( texture(material.diffuse, TexCoords) );
	vec3 specular = light.specular * spec * vec3( texture(material.specular, TexCoords) );

	diffuse *= attenuation;
	specular *= attenuation;

	diffuse *= intensity;
	specular *= intensity;

	return ( diffuse + specular );
    }
    else
    {
	return vec3(0.0);
    }
}
