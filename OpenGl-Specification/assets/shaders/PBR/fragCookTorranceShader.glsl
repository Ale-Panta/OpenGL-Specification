#version 430

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

layout (binding = 0) uniform sampler2D aAlbedoMap;
layout (binding = 1) uniform sampler2D aAOMap;
layout (binding = 2) uniform sampler2D aHeightMap;
layout (binding = 3) uniform sampler2D aMetallicMap;
layout (binding = 4) uniform sampler2D aNormalMap;
layout (binding = 5) uniform sampler2D aRoughnessMap;
layout (binding = 6) uniform sampler2DShadow aDepthMap;

layout (std140, binding = 24) uniform CameraProperties
{
	mat4 CamModelMat;
	mat4 CamViewMat;
	mat4 CamProjMat;
	vec4 CamPos;
	vec4 CamDir;
};

layout (std140, binding = 25) uniform LightProperties
{
	mat4 LightModelMat;
	mat4 LightViewMat;
	mat4 LightProjMat;
	vec4 LightPos;
	vec4 LightDir;
	vec4 LightColor;
	vec4 LightAmbient;
};

uniform mat4 uModel;
uniform float uTilingFactor;
uniform float uDisplacementFactor;

in VS_FS_INTERFACE
{
	vec4 ShadowCoord;
	vec3 Position;
	vec2 UV;
	mat3 TBN;
} Fragment;

out vec4 Color;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);
vec3 CalcNewNormal();

// PCF from GPU Gems NVIDIA
float offset_lookup(sampler2DShadow map, vec4 loc, vec2 offset)
{
	return textureProj(map, vec4(loc.xy + offset * vec2(0.0009765, 0.0009765) * loc.w, loc.z, loc.w));
}

void main()
{
	vec3 albedo = texture(aAlbedoMap, Fragment.UV).rgb;
	albedo.r = pow(albedo.r, 2.2);
	albedo.g = pow(albedo.g, 2.2);
	albedo.b = pow(albedo.b, 2.2);
	float metallic = texture(aMetallicMap, Fragment.UV).r;
	float roughness = texture(aRoughnessMap, Fragment.UV).r;
	float ambientOcclusion = texture(aAOMap, Fragment.UV).r;
	vec3 normal = CalcNewNormal();
	
	vec3 n = normalize(normal);
	vec3 v = normalize(CamPos.xyz - Fragment.Position);

	vec3 fo = vec3(0.04);
	fo = mix(fo, albedo, metallic);

	// Reflectance equation
	vec3 l = normalize(LightDir.xyz);
	vec3 h = normalize(v + l);
	
	float shadowCoeff = 0;
	float x, y;
	float countOfSample = 0;

	for (y = -1.5; y <= 1.5; y += 1.0)
	{
	  for (x = -1.5; x <= 1.5; x += 1.0)
	  {
		shadowCoeff += offset_lookup(aDepthMap, Fragment.ShadowCoord, vec2(x, y));
		countOfSample++;
	  }
	}

	shadowCoeff /= countOfSample;

	// float distance = length(LightPos.xyz - Position);
	// float attenuation = 1.0 / pow(distance, 2);
	vec3 radiance = LightColor.rgb * LightColor.a * shadowCoeff;

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


// For the normal distribution function (NDF), we found Disney's choice of GGX/Trowbridge-Retiz to
// be well worth the cost. The additional expense over using Blinn-Phong is fairly small, and the distinct,
// natural apperance produced by the longer "tail" appealed to our artist. We also adopted Disney's reparameterization
// of a = pow(Roughness, 2).
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
	
	// r can be calculated also like this to reduce the "hotness" ny remapping roughness.
	// This adjustement is only used for analytic light source; if applied to image-based lighting,
	// the result at glatcing angles will be much too dark
	// float r = pow((roughness + 1.0) / 2, 2);
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


// For Fresnel, we using Schlick's approximation, but with a minor modification
// we use a Spherical Gaussian approximation to replace the power. It is slightly
// more efficient to calculate and the difference is impredictable.
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 CalcNewNormal()
{
	// TBN matrix to convert to camera space
	vec3 retrievedNormal = texture(aNormalMap, Fragment.UV).xyz;
	retrievedNormal = normalize(retrievedNormal * 2.0 - 1.0);

	vec3 newNormal = normalize(Fragment.TBN * retrievedNormal);
	
	return newNormal;
}