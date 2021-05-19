#version 440 core

layout (binding = 0) uniform sampler2DShadow uDepthTexture;

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
	mat4 LightModelMat;
	mat4 LightViewMat;
	mat4 LightProjMat;
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightAmbient;
};

// --- End layout uniform blocks --------------------------------------------------------------------------------------

in vec4 ShadowCoord;
in vec3 WorldCoord;
in vec3 EyeCoord;
in vec3 Normal;

layout (location = 0) out vec4 color;

// PCF from GPU Gems NVIDIA
float offset_lookup(sampler2DShadow map, vec4 loc, vec2 offset)
{
	return textureProj(map, vec4(loc.xy + offset * vec2(0.0009765, 0.0009765) * loc.w, loc.z, loc.w));
}

void main()
{
	vec3 N = normalize(Normal);
	vec3 L = normalize(LightPos.xyz - WorldCoord);
	float LdotN = dot(N, L);
	vec3 R = reflect(-L, N);

	float diffuse = max(LdotN, 0.0);
	float specular = max(pow(dot(normalize(-EyeCoord), R), 25.0), 0.0);

	float shadowCoeff = 0;
	float x, y;
	float countOfSample = 0;

	for (y = -1.5; y <= 1.5; y += 1.0)
	{
	  for (x = -1.5; x <= 1.5; x += 1.0)
	  {
		shadowCoeff += offset_lookup(uDepthTexture, ShadowCoord, vec2(x, y));
		countOfSample++;
	  }
	}

	shadowCoeff /= countOfSample;

	color = vec4(vec3(0.1) + shadowCoeff * (vec3(0.3, 0.6, 0.8) * vec3(0.3) + vec3(1.0) * vec3(0.8)), 1.0);
	//color = vec4(mix(vec3(0.0), vec3(0.3, 0.6, 0.8) * vec3(0.3) + vec3(1.0) * vec3(0.8), shadowCoeff), 1.0);
}