#include "Material.h"

using namespace glm;
using namespace std;

namespace OpenGL
{
	Material::Material(const char* vertShaderSrc, const char* fragShaderSrc)
	{
		mShader = std::make_shared<Shader>(vertShaderSrc, fragShaderSrc);
	}

	Material::Material(const Material& material)
	{
		mShader = material.mShader;

		for (auto fParam : material.mFloatParameters)
			mFloatParameters.emplace_back(fParam);		// Construct a new object and push back
		for (auto v3Param : material.mVec3Parameters)
			mVec3Parameters.emplace_back(v3Param);		// Construct a new object and push back
		for (auto m4Param : material.mMat4Parameters)
			mMat4Parameters.emplace_back(m4Param);		// Construct a new object and push back
		for (auto tParam : material.mTextureParameters)
			mTextureParameters.push_back(tParam);		// Push the object directly because it's a pointer
		for (auto m4PtrParam : material.mMat4PtrParameters)
			mMat4PtrParameters.push_back(m4PtrParam);		// Push the object directly because it's a pointer
	}

	void Material::CommitToProgram()
	{
		if (mShader)
		{
			for (auto fParam : mFloatParameters)
				mShader->SetUniformFloat(fParam->GetMetaName().c_str(), fParam->GetValue());
			for (auto v3Param : mVec3Parameters)
				mShader->SetUniformVec3(v3Param->GetMetaName().c_str(), v3Param->GetValue());
			for (auto m4Param : mMat4Parameters)
				mShader->SetUniformMatrix4(m4Param->GetMetaName().c_str(), m4Param->GetValue());
			for (auto v3PtrParam : mVec3PtrParameters)
			{
				if (v3PtrParam)
					mShader->SetUniformVec3(v3PtrParam->GetMetaName().c_str(), *v3PtrParam->GetValue());
			}
			for (auto m4PtrParam : mMat4PtrParameters)
			{
				if (m4PtrParam)
					mShader->SetUniformMatrix4(m4PtrParam->GetMetaName().c_str(), *m4PtrParam->GetValue());
			}

			int textureIndex = 0;
			for (auto tParam : mTextureParameters)
			{
				glActiveTexture(GL_TEXTURE0 + textureIndex);
				glBindTexture((GLenum)tParam->GetValue()->GetTextureType(), *tParam->GetValue());
				//shader->SetUniformInt(tParam->GetMetaName().c_str(), *tParam->GetValue()); // Currently not used this line
				textureIndex++;
			}
		}
	}

	void Material::UnbindHeavyAssetsToProgram()
	{
		int textureIndex = 0;
		for (auto tParam : mTextureParameters)
		{
			glBindTexture((int)tParam->GetValue()->GetTextureType(), 0);
			textureIndex++;
		}
	}

	// Pewter material
	//AddVec3Param(std::make_shared<Vec3MatParam>("GlobalAmbient", vec3(.7f, .7f, .7f)));
	//AddVec3Param(std::make_shared<Vec3MatParam>("Ambient", vec3(.11f, .06f, .11f)));
	//AddVec3Param(std::make_shared<Vec3MatParam>("Diffuse", vec3(.43f, .47f, .54f)));
	//AddVec3Param(std::make_shared<Vec3MatParam>("Specular", vec3(.33f, .33f, .52f)));
	//AddFloatParam(std::make_shared<FloatMatParam>("Shininess", 9.85f));

	// Gold material
	//m_GlobalAmbient = vec3(.7f, .7f, .7f);
	//m_Ambient = vec3(.24725f, .1995f, .0745f);
	//m_Diffuse = vec3(.75164f, .60648f, .22648f);
	//m_Specular = vec3(.62828f, .5558f, .36607f);
	//m_Shininess = 51.2f;

	// Silver material
	//m_GlobalAmbient = vec3(.7f, .7f, .7f);
	//m_Ambient = vec3(.19225f, .19225f, .19225f);
	//m_Diffuse = vec3(.50754f, .50754f, .50754f);
	//m_Specular = vec3(.50827f, .50827f, .50827f);
	//m_Shininess = 51.2f;

	// Bronze material
	//m_GlobalAmbient = vec3(.7f, .7f, .7f);
	//m_Ambient = vec3(.2125f, .2125f, .2125f);
	//m_Diffuse = vec3(.7140f, .4284f, .1814f);
	//m_Specular = vec3(.3936f, .2719f, .1667f);
	//m_Shininess = 25.6f;
}