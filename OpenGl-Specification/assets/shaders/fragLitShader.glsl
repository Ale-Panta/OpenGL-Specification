#version 430

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform float uTime;		// current time.
uniform mat4 uModel;		// Model View matrix.
uniform mat4 uView;			// Model View matrix.
uniform mat4 uProjection;	// Projection matrix.

uniform vec3 uViewPos;

uniform Material uMaterial;
uniform Light uLight;

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;

out vec4 color;

void main(void)
{ 
	// ambient
	vec3 ambient = uLight.ambient * uMaterial.ambient;
	
	// diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(uLight.position - Position);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = uLight.diffuse * (diff * uMaterial.diffuse);
	
	// specular
	vec3 viewDir = normalize(uViewPos - Position);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterial.shininess);
	vec3 specular = uLight.specular * spec * uMaterial.specular;
	
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0); 
}