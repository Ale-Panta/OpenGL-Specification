#pragma once

#include "Shader.h"
#include "Texture.h"

#include <glm/glm.hpp>

#include <memory>
#include <vector>
#include <string>

namespace OpenGL
{
	/**
	 * This is a template structure that represent any possible parameter used in materials.
	 * It provides: Generic Value, Name, Prefix.
	 */
	template <class T>
	class MaterialParameter
	{
	public:
		MaterialParameter() = default;
		MaterialParameter<T>(const std::string name, T value) : Name(name), Value(value) { }
		~MaterialParameter() = default;

		/** Returns a reference to generic value. #Note: this return the address to a copy-value */
		const T& GetValue() const { return Value; }

		/** Returns the reference to name. #Note: this return the address to a copy-value */
		const std::string& GetName() const { return Name; }

		/** Returns a copy-string to prefix + name. */
		const std::string GetMetaName() const { return Prefix + Name; }

		T Value;
		std::string Name = "Default";
		const std::string Prefix = "";
	};

	// Syntactic sugar
	using FloatMatParam		= MaterialParameter<float>;
	using Vec3MatParam		= MaterialParameter<glm::vec3>;
	using Mat4MatParam		= MaterialParameter<glm::mat4>;
	using Vec3PtrMatParam	= MaterialParameter<const glm::vec3*>;
	using Mat4PtrMatParam	= MaterialParameter<const glm::mat4*>;
	using TextureMatParam	= MaterialParameter<std::shared_ptr<Texture>>;

	/**
	 * Material asset is designed in a way that any parameter can be added dynamically.
	 * This way the user can add any parameter as he want after the material instance is created.
	 * This as some benefits like avoid inheritance to specify some pecular materials.
	 * Material contains heavy (textures, shaders, ...) and light (float, vec3, mat4, ...) parameters.
	 * #Note: must be created before binding textures or parameters.
	 */
	class Material
	{
	public:
		Material() = default;

		/** Constructing material will construct the shader automatically */
		Material(const char* vertShaderSrc, const char* fragShaderSrc);

		/** Copy-constructor is usefull to prevent copy heavy parameters */
		Material(const Material& material);

		/** Commit to program shader all parameters subscribed. It will be called just before drawing geometry */
		virtual void CommitToProgram();

		/** Unbind all heavy parameters subscribed from the program shader. It will be called just after drawing geometry */
		virtual void UnbindHeavyAssetsToProgram();

		/** Create a shared pointer MaterialParamter<float> */
		void AddFloatParam(std::shared_ptr<FloatMatParam> floatParameter)		{ mFloatParameters.push_back(floatParameter); }

		/** Create a shared pointer MaterialParamter<glm::vec3> */
		void AddVec3Param(std::shared_ptr<Vec3MatParam> vec3Parameter)			{ mVec3Parameters.push_back(vec3Parameter); }

		/** Create a shared pointer MaterialParamter<glm::mat4> */
		void AddMat4Param(std::shared_ptr<Mat4MatParam> mat4Parameter)			{ mMat4Parameters.push_back(mat4Parameter); }

		/** Create a shared pointer MaterialParamter<glm::vec3*>. Use it to bind values that change over time */
		void AddVec3PtrParam(std::shared_ptr<Vec3PtrMatParam> vec3PtrParameter) { mVec3PtrParameters.push_back(vec3PtrParameter); }

		/** Create a shared pointer MaterialParamter<glm::mat4*>. Use it to bind view, model, projection matrix for example */
		void AddMat4PtrParam(std::shared_ptr<Mat4PtrMatParam> mat4PtrParameter) { mMat4PtrParameters.push_back(mat4PtrParameter); }

		/** Create a shared pointer MaterialParamter<std::shared_ptr<Texture>> */
		void AddTextureParam(std::shared_ptr<TextureMatParam> textureParameter) { mTextureParameters.push_back(textureParameter); }

		operator GLuint() const { return *mShader; }

	private:
		/** I want the shader to be shared pointer because copying material can happens */
		std::shared_ptr<Shader>							mShader;
		std::vector<std::shared_ptr<FloatMatParam>>		mFloatParameters;
		std::vector<std::shared_ptr<Vec3MatParam>>		mVec3Parameters;
		std::vector<std::shared_ptr<Mat4MatParam>>		mMat4Parameters;
		std::vector<std::shared_ptr<Vec3PtrMatParam>>	mVec3PtrParameters;
		std::vector<std::shared_ptr<Mat4PtrMatParam>>	mMat4PtrParameters;
		std::vector<std::shared_ptr<TextureMatParam>>	mTextureParameters;
	};
}