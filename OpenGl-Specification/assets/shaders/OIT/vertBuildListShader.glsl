#version 440 core

// --- Begin layout vertex data ---------------------------------------------------------------------------------------

layout (location = 0) in vec3 aPosition;
layout (location = 2) in vec3 aNormal;

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

// --- End layout uniform blocks --------------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// --- Begin local program uniforms -----------------------------------------------------------------------------------

uniform mat4 uModel;
uniform vec4 uColor;

// --- End local program uniforms -------------------------------------------------------------------------------------

out vec4 surface_color;
out vec3 frag_position;
out vec3 frag_normal;

void main()
{
	surface_color = uColor;
	frag_position = (uModel * vec4(aPosition, 1.0)).xyz;
	frag_normal = mat3(uModel * CamViewMat) * aNormal;
	gl_Position = CamProjMat * CamViewMat * uModel * vec4(aPosition, 1.0);
}