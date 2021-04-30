#pragma once
#include "../../src/Scene.h"
#include "../../src/Camera.h"
#include "../../src/Shader.h"
#include "../../src/Mesh.h"
#include "../../src/Light.h"
#include <stack>

namespace OpenGL
{
	class PBRScene : public Scene
	{
	public:
		PBRScene() = default;

		void BeginScene(GLFWwindow* context) override;
		void RenderScene(GLFWwindow* context, double currentTime) override;
		void RenderSkyBox(GLFWwindow* context, double currentTime) override;
		void RenderShadow(GLFWwindow* context, double currentTime) override;
		void RenderGeometry(GLFWwindow* context, double currentTime) override;

	private:
		std::shared_ptr<Camera> m_Camera;
		Light* LightSource;
		
		std::shared_ptr<Shader> m_PBRShader;

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

		GLuint m_UBOLightPrties = 0;
		GLuint m_UBOCamPrties = 0;
		GLuint m_FBOShadow = 0;
	};
}