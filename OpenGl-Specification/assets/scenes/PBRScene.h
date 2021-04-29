#pragma once
#include "../../src/Scene.h"
#include "../../src/Camera.h"
#include "../../src/Shader.h"
#include "../../src/Material.h"
#include "../../src/Mesh.h"
#include <stack>

namespace OpenGL
{
	struct LitProperties
	{
		glm::mat4 ViewMat		= glm::mat4(1.0f);	// 0
		glm::mat4 ModelMat		= glm::mat4(1.0f);	// 64
		glm::mat4 ProjMat		= glm::mat4(1.0f);	// 128
		glm::vec4 CamPos		= glm::vec4(1.0f);	// 192
		glm::vec4 LightPos		= glm::vec4(1.0f);	// 208
		glm::vec4 LightColor	= glm::vec4(1.0f);	// 224
		glm::vec4 LightAmbient	= glm::vec4(1.0f);	// 240
	};

	class PBRScene : public Scene
	{

	public:
		void BeginScene(GLFWwindow* context) override;
		void RenderScene(GLFWwindow* context, double currentTime) override;
		void RenderSkyBox(GLFWwindow* context, double currentTime) override;
		void RenderShadow(GLFWwindow* context, double currentTime) override;
		void RenderGeometry(GLFWwindow* context, double currentTime) override;

	private:
		std::shared_ptr<Camera> m_Camera;
		std::shared_ptr<glm::vec3> m_LightPosition;
		std::shared_ptr<glm::vec3> m_LightColor;
		
		std::shared_ptr<Shader> m_PBRShader;
		std::shared_ptr<Material> m_BronzeMaterial;

		std::shared_ptr<Mesh> m_SphereCD;
		std::shared_ptr<Mesh> m_SpherePG;
		std::shared_ptr<Mesh> m_SphereSS;
		std::shared_ptr<Mesh> m_SphereVM;
		std::stack<glm::mat4> m_ModelMatStack;

		// --- Textures ---
 
		// Begin Cavern Deposit
		Texture2D* m_CD_Albedo;
		Texture2D* m_CD_AO;
		Texture2D* m_CD_Height;
		Texture2D* m_CD_Metallic;
		Texture2D* m_CD_Normal;
		Texture2D* m_CD_Roughness;
		// End Cavern Deposit

		// Begin Pirate Gold
		Texture2D* m_PG_Albedo;
		Texture2D* m_PG_AO;
		Texture2D* m_PG_Height;
		Texture2D* m_PG_Metallic;
		Texture2D* m_PG_Normal;
		Texture2D* m_PG_Roughness;
		// End Pirate Gold

		// Begin Space Ship
		Texture2D* m_SS_Albedo;
		Texture2D* m_SS_AO;
		Texture2D* m_SS_Height;
		Texture2D* m_SS_Metallic;
		Texture2D* m_SS_Normal;
		Texture2D* m_SS_Roughness;
		// End Space Ship

		// Begin Vented Metal
		Texture2D* m_VM_Albedo;
		Texture2D* m_VM_AO;
		Texture2D* m_VM_Height;
		Texture2D* m_VM_Metallic;
		Texture2D* m_VM_Normal;
		Texture2D* m_VM_Roughness;
		// End Vented Metal

		GLint m_LitPropertiesLocation = 0;
		GLuint m_UBO = 0;
		GLuint m_FBOShadow = 0;
	};
}