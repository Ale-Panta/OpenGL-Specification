#include "OITScene.h"
#include "../meshes/Sphere.h"
#include "../meshes/Plane.h"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

using namespace glm;
using namespace std;

namespace OpenGL
{
	OITScene::OITScene(float fovy, float aspectRatio, float near, float far)
	{
		m_Camera = new Camera(vec4(-5.0f, 0.0f, 4.0f, 0.0f), fovy, aspectRatio, near, far);
		m_LightSource = new Light(vec4(0.0f, 0.0f, 5.0f, 0.0f), vec4(0.0f), vec4(50.0f, 45.0f, 43.0f, 1.0f), vec4(1.0f));
	}


	OITScene::~OITScene()
	{
		delete m_LitShader;
		delete m_UnlitShader;
		delete m_BuildListShader;
		delete m_ResolveListShader;

		delete m_SphereCD;
		delete m_SpherePG;
		delete m_SphereSS;
		delete m_SphereVM;
		delete m_Plane;

		delete m_Camera;
		delete m_LightSource;
	}

	void OITScene::BeginScene(GLFWwindow* context)
	{
		m_SphereCD = new Sphere(32);
		m_SpherePG = new Sphere(32);
		m_SphereSS = new Sphere(32);
		m_SphereVM = new Sphere(256);
		m_Plane = new Plane();

		m_LitShader = new Shader("assets/shaders/PBRCookTorrance/vertCookTorranceShader.glsl", "assets/shaders/PBRCookTorrance/fragCookTorranceShader.glsl");
		m_UnlitShader = new Shader("assets/shaders/Unlit/vertUnlitShader.glsl", "assets/shaders/Unlit/fragUnlitShader.glsl");
		m_BuildListShader = new Shader("assets/shaders/OIT/vertBuildListShader.glsl", "assets/shaders/OIT/fragBuildListShader.glsl");
		m_ResolveListShader = new Shader("assets/shaders/OIT/vertResolveListShader.glsl", "assets/shaders/OIT/fragResolveListShader.glsl");

		// Create head pointer texture
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &head_pointer_texture);
		glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, MAX_TEX_WIDTH, MAX_TEX_HEIGHT, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindImageTexture(0, head_pointer_texture, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);

		// Create buffer for clearing the head pointer texture
		GLuint* data;

		glGenBuffers(1, &head_pointer_clear_buffer);
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
		glBufferData(GL_PIXEL_UNPACK_BUFFER, MAX_TEX_WIDTH * MAX_TEX_HEIGHT * sizeof(GLuint), NULL, GL_STATIC_DRAW);
		data = (GLuint*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
		memset(data, 0x00, MAX_TEX_WIDTH * MAX_TEX_HEIGHT * sizeof(GLuint));
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
		
		// Create the atomic counter buffer
		glGenBuffers(1, &atomic_counter_buffer);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);

		// Create the linked list storage buffer
		glGenBuffers(1, &linked_list_buffer);
		glBindBuffer(GL_TEXTURE_BUFFER, linked_list_buffer);
		glBufferData(GL_TEXTURE_BUFFER, MAX_TEX_WIDTH * MAX_TEX_HEIGHT * 3 * sizeof(vec4), NULL, GL_DYNAMIC_COPY);
		glBindBuffer(GL_TEXTURE_BUFFER, 0);

		// Bind it to a texture (for use as a TBO)
		glGenTextures(1, &linked_list_texture);
		glBindTexture(GL_TEXTURE_BUFFER, linked_list_texture);
		glTexBuffer(GL_TEXTURE_BUFFER, GL_RGBA32UI, linked_list_buffer);
		glBindTexture(GL_TEXTURE_BUFFER, 0);

		glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

		glClearDepth(1.0f);

		// Retrieve uniform block location
		GLint camPrtiesLocation = glGetUniformBlockIndex(*m_LitShader, "CameraProperties");
		glUniformBlockBinding(*m_LitShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_UnlitShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_BuildListShader, camPrtiesLocation, 24);
		glUniformBlockBinding(*m_ResolveListShader, camPrtiesLocation, 24);

		// Initialize uniform block
		glGenBuffers(1, &m_UBOCamPrties);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOCamPrties);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(CameraProperties), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 24, m_UBOCamPrties);

		// Retrieve uniform block location
		GLint lightPrtiesLocation = glGetUniformBlockIndex(*m_LitShader, "LightProperties");
		glUniformBlockBinding(*m_LitShader, lightPrtiesLocation, 25);

		// Initialize uniform block
		glGenBuffers(1, &m_UBOLightPrties);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOLightPrties);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(LightProperties), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferBase(GL_UNIFORM_BUFFER, 25, m_UBOLightPrties);

		m_Camera->UpdateUniformBlock(m_UBOCamPrties);			// Fill camera uniform block buffer
		m_LightSource->UpdateUniformBlock(m_UBOLightPrties);	// Fill light uniform block buffer
	}

	void OITScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		// Clear buffers
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Reset values...
		glDisable(GL_BLEND);

		// Setting to draw skybox...
		RenderSkyBox(context, currentTime);

		// Settings to draw shadow...
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_CCW);

		RenderShadow(context, currentTime);

		// Settings to draw opaque...
		glDepthMask(GL_TRUE);

		RenderOpaque(context, currentTime);

		// Settings to draw transparency...

		// Reset atomic counter
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
		data = (GLuint*)glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_WRITE_ONLY);
		data[0] = 0;
		glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);

		// Clear head-pointer image
		glBindBuffer(GL_PIXEL_UNPACK_BUFFER, head_pointer_clear_buffer);
		glBindTexture(GL_TEXTURE_2D, head_pointer_texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1080, 1080, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Bind head-pointer image for read-write
		glBindImageTexture(0, head_pointer_texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

		// Bind linked-list buffer for write
		glBindImageTexture(1, linked_list_texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32UI);

		glDisable(GL_CULL_FACE);
		glDepthMask(GL_FALSE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		RenderTrasparency(context, currentTime);

		m_Plane->Draw(*m_ResolveListShader);
	}

	void OITScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
	}

	void OITScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void OITScene::RenderOpaque(GLFWwindow* context, double currentTime)
	{
		static float lightDistance = 5.0f;
		m_LightSource->SetWorldPos(vec4(sin(currentTime / 5.0f) * lightDistance, 0.0f, cos(currentTime / 5.0f) * lightDistance, 0.0f));
		m_LightSource->UpdateUniformBlock(m_UBOLightPrties);

		m_UnlitShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(1.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.2f, 1.2f, 1.2f)));
		m_UnlitShader->SetUniformVec4("uColor", vec4(0.8f, 0.2f, 0.2f, 1.0f));
		m_SphereCD->Draw(*m_UnlitShader);
	}

	void OITScene::RenderTrasparency(GLFWwindow* context, double currentTime)
	{
		static float lightDistance = 5.0f;
		m_LightSource->SetWorldPos(vec4(sin(currentTime / 5.0f) * lightDistance, 0.0f, cos(currentTime / 5.0f) * lightDistance, 0.0f));
		m_LightSource->UpdateUniformBlock(m_UBOLightPrties);

		m_BuildListShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f, cos(currentTime) - 1.0f, 0.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(1.0f, 0.3f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.0f, 1.0f)));
		m_BuildListShader->SetUniformVec4("uColor", vec4(0.0f, 0.5f, 0.0f, sin(currentTime * 0.6f) * 1.0f * 0.5f + 0.5f));
		m_SpherePG->Draw(*m_BuildListShader);

		m_BuildListShader->SetUniformMatrix4("uModel", translate(mat4(1.0f), vec3(0.0f, 0.0f, sin(currentTime) * 1.0f)) * rotate(mat4(1.0f), (float)radians(currentTime * 0.0f), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.8f, 0.8f, 0.8f)));
		m_BuildListShader->SetUniformVec4("uColor", vec4(0.0f, 0.5f, 0.5f, 0.8f));
		m_SphereSS->Draw(*m_BuildListShader);

		/*
		*/
	}
}