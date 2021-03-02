/*! \file openGLShaderProgram.h
*
* \brief An OpenGL Shader program
*
* \author Daniel Bullin
*
*/
#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "independent/rendering/shaders/shaderProgram.h"

namespace Engine
{
	/*! \class OpenGLShaderProgram
	* \brief An OpenGL Shader. Create a shader through the build function
	*/
	class OpenGLShaderProgram : public ShaderProgram
	{
	private:
		uint32_t constructShaders(const int shaderType, const char* filePath); //!< Construct individual shader objects for the program
	public:
		OpenGLShaderProgram(const std::string& shaderName); //!< Constructor
		~OpenGLShaderProgram(); //!< Destructor

		void build(VertexArray* vertexArray, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, const std::string& tessControlPath, const std::string& tessEvalPath) override; //!< Create a shader program based on the files passed
		void setUniforms(const std::vector<std::string>& uniformNames) override; //!< Set all uniform locations
		void setUniformBuffers(const std::unordered_map<std::string, UniformBuffer*>& uniformBuffers) override; //!< Set the uniform buffers used by this shader
		void start() override; //!< Start the shader
		void stop() override; //!< Stop using the shader
		void destroy() override; //!< Destroy the shader
		void sendInt(const std::string& uniformName, const int value) override; //!< Upload integer to shader
		void sendIntArray(const std::string& uniformName, int32_t* values, const uint32_t count) override; //!< Upload an array of integers to shader
		void sendBool(const std::string& uniformName, const bool value) override; //!< Upload bool to shader
		void sendFloat(const std::string& uniformName, const float value) override; //!< Upload float to shader
		void sendVec2(const std::string& uniformName, const glm::vec2& value) override; //!< Upload vec2 to shader
		void sendVec3(const std::string& uniformName, const glm::vec3& value) override; //!< Upload vec3 to shader
		void sendVec4(const std::string& uniformName, const glm::vec4& value) override; //!< Upload vec4 to shader
		void sendMat3(const std::string& uniformName, const glm::mat3& mat) override; //!< Upload mat3 to shader
		void sendMat4(const std::string& uniformName, const glm::mat4& mat) override; //!< Upload mat4 to shader
		void printDetails() override; //!< Print the resource details
	};
}
#endif