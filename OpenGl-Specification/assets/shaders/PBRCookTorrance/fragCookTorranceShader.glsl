#version 430

// --- Begin layout uniform textures ----------------------------------------------------------------------------------

layout (binding = 0) uniform sampler2D uAlbedoMap;
layout (binding = 1) uniform sampler2D uAOMap;
layout (binding = 2) uniform sampler2D uHeightMap;
layout (binding = 3) uniform sampler2D uMetallicMap;
layout (binding = 4) uniform sampler2D uNormalMap;
layout (binding = 5) uniform sampler2D uRoughnessMap;

// --- End layout uniform textures ------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin layout uniform blocks ------------------------------------------------------------------------------------

layout (std140, binding = 24) uniform CameraProperties
{
	mat4 CamModelMat;
	mat4 CamViewMat;
	mat4 CamProjMat;
	vec4 CamPos;
};

layout (std140, binding = 25) uniform LightProperties
{
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightAmbient;
};

// --- End layout uniform blocks --------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin local program uniforms -----------------------------------------------------------------------------------

uniform mat4 uModel;
uniform float uTilingFactor;
uniform float uDisplacementFactor;

// --- End local program uniforms -------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin in values ------------------------------------------------------------------------------------------------

in vec3 Position;
in vec2 UV;
in mat3 TBN;

// --- End in values --------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin out values -----------------------------------------------------------------------------------------------

out vec4 Color;

// --- End out values -------------------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin others values --------------------------------------------------------------------------------------------

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 CalcNewNormal();

// --- End others values ----------------------------------------------------------------------------------------------

void main()
{
	vec3 albedo = texture(uAlbedoMap, UV).rgb;
	albedo.r = pow(albedo.r, 2.2);
	albedo.g = pow(albedo.g, 2.2);
	albedo.b = pow(albedo.b, 2.2);
	float metallic = texture(uMetallicMap, UV).r;
	float roughness = texture(uRoughnessMap, UV).r;
	float ambientOcclusion = texture(uAOMap, UV).r;
	vec3 normal = CalcNewNormal();
	
	vec3 n = normalize(normal);
	vec3 v = normalize(CamPos.xyz - Position);

	vec3 fo = vec3(0.04);
	fo = mix(fo, albedo, metallic);

	// Reflectance equation
	vec3 l = normalize(LightPos.xyz - Position);
	vec3 h = normalize(v + l);
	
	float distance = length(LightPos.xyz - Position);
	float attenuation = 1.0 / pow(distance, 2);
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
	vec3 specular = numerator / max(denominator, 1.0);

	// Add to outgoing radiance Lo
	float nDotL = max(dot(n, l), 0.0);
	vec3 lo = (kD * albedo / PI + specular) * radiance * nDotL;

	vec3 ambientColor = vec3(0.03, 0.0295, 0.02903);
	vec3 ambient = ambientColor * albedo * ambientOcclusion;
	vec3 outColor = ambient + lo;

	outColor = outColor / (outColor + vec3(1.0));
	outColor = pow(outColor, vec3(1.0 / 2.2));	// Gamma correction

	Color = vec4(outColor, 1.0);
}

/**
 * For the normal distribution function (NDF), we found Disney's choice of GGX/Trowbridge-Retiz to
 * be well worth the cost. The additional expense over using Blinn-Phong is fairly small, and the distinct,
 * natural apperance produced by the longer "tail" appealed to our artist. We also adopted Disney's reparameterization
 * of a = pow(Roughness, 2).
 */
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = pow(roughness, 2);
    float a2     = pow(a, 2);
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = pow(NdotH, 2);
	
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * pow(denom, 2);
	
    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	/*
	* r can be calculated also like this to reduce the "hotness" ny remapping roughness.
	* This adjustement is only used for analytic light source; if applied to image-based lighting,
	* the result at glatcing angles will be much too dark
	* float r = pow((roughness + 1.0) / 2, 2);
	*/
    float r = pow(roughness + 1.0, 2);
    float k = r / 8.0;

    return NdotV / (NdotV * (1.0 - k) + k);
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

/**
 * For Fresnel, we using Schlick's approximation, but with a minor modification
 * we use a Spherical Gaussian approximation to replace the power. It is slightly
 * more efficient to calculate and the difference is impredictable.
 */
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 CalcNewNormal()
{
	// TBN matrix to convert to camera space
	vec3 retrievedNormal = texture(uNormalMap, UV).xyz;
	retrievedNormal = normalize(retrievedNormal * 2.0 - 1.0);

	vec3 newNormal = normalize(TBN * retrievedNormal);
	
	return newNormal;
}