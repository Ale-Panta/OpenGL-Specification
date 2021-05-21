#version 440 core

layout (binding = 0) uniform sampler2DShadow aDepthTexture;

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

in VS_FS_INTERFACE
{
	vec4 ShadowCoord;
	vec3 WorldCoord;
	vec3 EyeCoord;
	vec3 Normal;
} Fragment;

out vec4 Color;

// PCF from GPU Gems NVIDIA
float offset_lookup(sampler2DShadow map, vec4 loc, vec2 offset)
{
	return textureProj(map, vec4(loc.xy + offset * vec2(0.0009765, 0.0009765) * loc.w, loc.z, loc.w));
}

void main()
{
	vec3 N = normalize(Fragment.Normal);
	vec3 L = normalize(LightDir.xyz);
	float LdotN = dot(N, L);
	vec3 R = reflect(-L, N);

	float diffuse = max(LdotN, 0.0);
	float specular = max(pow(dot(normalize(-Fragment.EyeCoord), R), 25.0), 0.0);

	float shadowCoeff = 0;
	float x, y;
	float countOfSample = 0;

	for (y = -1.5; y <= 1.5; y += 1.0)
	{
	  for (x = -1.5; x <= 1.5; x += 1.0)
	  {
		shadowCoeff += offset_lookup(aDepthTexture, Fragment.ShadowCoord, vec2(x, y));
		countOfSample++;
	  }
	}

	shadowCoeff /= countOfSample;

	Color = vec4(vec3(0.1) + shadowCoeff * (vec3(0.3, 0.6, 0.8) * diffuse + vec3(1.0) * vec3(0.3)), 1.0);
}