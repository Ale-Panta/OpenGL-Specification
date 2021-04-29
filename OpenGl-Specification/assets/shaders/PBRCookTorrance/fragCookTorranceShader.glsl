#version 430

struct Material
{
	vec3 albedo;
	float metallic;
	float roughness;
	float ao;
};

layout (std140, binding = 13) uniform LitProperties
{
	mat4 CamModelMat;
	mat4 CamViewMat;
	mat4 CamProjMat;
	vec4 CamPos;
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightAmbient;
};

// PBR maps
layout (binding = 0) uniform sampler2D uAlbedoMap;
layout (binding = 1) uniform sampler2D uAOMap;
layout (binding = 2) uniform sampler2D uHeightMap;
layout (binding = 3) uniform sampler2D uMetallicMap;
layout (binding = 4) uniform sampler2D uNormalMap;
layout (binding = 5) uniform sampler2D uRoughnessMap;

// Matrices parameteres
uniform mat4 uModel;
uniform float uTilingFactor;
uniform Material uMaterial;

in vec3 Position;
in vec2 TexCoord;
in vec3 Normal;
in vec3 Tangent;

out vec4 Color;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 CalcNewNormal();

void main()
{
	vec3 albedo = texture(uAlbedoMap, TexCoord).rgb;
	albedo.r = pow(albedo.r, 2.2);
	albedo.g = pow(albedo.g, 2.2);
	albedo.b = pow(albedo.b, 2.2);
	float metallic = texture(uMetallicMap, TexCoord).r;
	float roughness = texture(uRoughnessMap, TexCoord).r;
	float ambientOcclusion = texture(uAOMap, TexCoord).r;
	vec3 normal = CalcNewNormal();
	
	vec3 n = normalize(normal);
	vec3 v = normalize(CamPos.xyz - Position);

	vec3 fo = vec3(0.04);
	fo = mix(fo, albedo, metallic);

	// Reflectance equation
	vec3 l = normalize(LightPos.xyz - Position);
	vec3 h = normalize(v + l);
	
	float distance = length(LightPos.xyz - Position);
	float attenuation = 1.0 / (distance * distance);
	vec3 radiance = LightColor.rgb * attenuation;

	// Cook-Torrance BDRDF
	float ndf = DistributionGGX(n, h, roughness);
	float g = GeometrySmith(n, v, l, roughness);
	vec3 f = FresnelSchlick(max(dot(h, v), 0.0), fo);

	vec3 kS = f;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	vec3 numerator = ndf * g * f;
	float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0);
	vec3 specular = numerator / max(denominator, 0.001);

	// Add to outgoing radiance Lo
	float nDotL = max(dot(n, l), 0.0);
	vec3 lo = (kD * albedo / PI + specular) * radiance * nDotL;

	vec3 ambient = vec3(0.088) * albedo * ambientOcclusion;
	vec3 outColor = ambient + lo;

	outColor = outColor / (outColor + vec3(1.0));
	outColor = pow(outColor, vec3(1.0 / 2.2));	// Gamma correction

	Color = vec4(outColor, 1.0);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 CalcNewNormal()
{
	vec3 normal = normalize(Normal);
	vec3 tangent = normalize(Tangent);
	
	// Tangent is perpendicular to normal
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, -normal);
	
	// TBN matrix to convert to camera space
	vec3 retrievedNormal = texture(uNormalMap, TexCoord).xyz;
	retrievedNormal = normalize(retrievedNormal * 2.0 - 1.0);

	mat3 tbn = mat3(tangent, bitangent, normal);
	vec3 newNormal = tbn * retrievedNormal;
	newNormal = normalize(newNormal);
	return newNormal;
}