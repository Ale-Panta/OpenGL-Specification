#version 440 core

// --- Begin layout vertex data ---------------------------------------------------------------------------------------

layout (location = 0) in vec3 aPosition;

// --- End layout vertex data -----------------------------------------------------------------------------------------
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
	mat4 LightModelMat;
	mat4 LightViewMat;
	mat4 LightProjMat;
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightAmbient;
};

// --- End layout uniform blocks --------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin local program uniforms -----------------------------------------------------------------------------------

uniform mat4 uModel;

// --- End local program uniforms -------------------------------------------------------------------------------------

void main()
{
	gl_Position = LightProjMat * LightViewMat * uModel * vec4(aPosition, 1.0);
}