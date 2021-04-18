#include "PBRScene.h"
#include "../../src/Texture.h"
#include "../meshes/Sphere.h"
#include "../meshes/Cube.h"

using namespace glm;
using namespace std;

namespace OpenGL
{

	void PBRScene::BeginScene(GLFWwindow* context)
	{
		mCamera = make_shared<Camera>(vec3(.0f, .0f, 4.0f), .01f);
		mLightPosition = make_shared<vec3>(0.0f);
		mLightColor = make_shared<vec3>(50.0f);
		mSphere = make_shared<Sphere>(64);

		mCubeMap = std::make_shared<Cube>();

		mMaterial = make_shared<Material>("assets/shaders/PBRCookTorrance/vertCookTorranceShader.glsl", "assets/shaders/PBRCookTorrance/fragCookTorranceShader.glsl");
		// Material local properties
		mMaterial->AddFloatParam(make_shared<FloatMatParam>("uMaterial.ao", 1.0f));
		mMaterial->AddMat4Param(make_shared<Mat4MatParam>("uModel", translate(mat4(1.0f), vec3(.0f))));
		// Textures
		mMaterial->AddTextureParam(make_shared<TextureMatParam>("uAlbedoMap",		make_shared<Texture2D>("assets/textures/rustediron/rustediron2_basecolor.png")));
		mMaterial->AddTextureParam(make_shared<TextureMatParam>("uMetallicMap",		make_shared<Texture2D>("assets/textures/rustediron/rustediron2_metallic.png")));
		mMaterial->AddTextureParam(make_shared<TextureMatParam>("uRoughnessMap",	make_shared<Texture2D>("assets/textures/rustediron/rustediron2_roughness.png")));
		mMaterial->AddTextureParam(make_shared<TextureMatParam>("uNormalMap",		make_shared<Texture2D>("assets/textures/rustediron/rustediron2_normal.png")));
		// Camera pointer values
		mMaterial->AddMat4PtrParam(make_shared<Mat4PtrMatParam>("uView", mCamera->GetViewMatrixPtr()));
		mMaterial->AddMat4PtrParam(make_shared<Mat4PtrMatParam>("uProjection", mCamera->GetProjMatrixPtr()));
		mMaterial->AddVec3PtrParam(make_shared<Vec3PtrMatParam>("uCameraPosition", mCamera->GetPositionPtr()));
		// Light pointer values
		mMaterial->AddVec3PtrParam(make_shared<Vec3PtrMatParam>("uLight.color", mLightColor.get()));
		mMaterial->AddVec3PtrParam(make_shared<Vec3PtrMatParam>("uLight.position", mLightPosition.get()));
	}

	void PBRScene::RenderScene(GLFWwindow* context, double currentTime)
	{
		mCamera->ProcessInput(context);

		glClearColor(.05f, .05f, .05f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		RenderSkyBox(context, currentTime);
		RenderShadow(context, currentTime);
		RenderGeometry(context, currentTime);
	}

	void PBRScene::RenderSkyBox(GLFWwindow* context, double currentTime)
	{
	}

	void PBRScene::RenderShadow(GLFWwindow* context, double currentTime)
	{
	}

	void PBRScene::RenderGeometry(GLFWwindow* context, double currentTime)
	{
		mLightPosition->x = sin((float)currentTime) * 10.0f;
		mLightPosition->y = .0f;
		mLightPosition->z = cos((float)currentTime) * 10.0f;

		mCubeMap->Draw(*mMaterial);
		//mSphere->Draw(*mMaterial);
	}

}