/*! \file shaderProgram.h
*
* \brief An API agnostic shader program
*
* \author Daniel Bullin
*
*/
#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include "independent/core/common.h"

namespace Engine
{
	/*! \class ShaderProgram
	* \brief An API agnostic shader program
	*/
	class ShaderProgram
	{
	protected:
		uint32_t m_programID; //!< The shader program ID
		std::map<std::string, int> m_uniforms; //!< All the uniforms for the shader program
	public:
		virtual ~ShaderProgram() = default; //!< Destructor
		virtual void build(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, const std::string& tessControlPath, const std::string& tessEvalPath) = 0; //!< Create a shader program based on the files passed
			/*!< \param vertexPath a const std::string& - The vertex shader file path
				 \param fragmentPath a const std::string& - The fragment shader file path
				 \param geometryPath a const std::string& - The geometry shader file path 
				 \param tessControlPath a const std::string& - The tessellation control shader file path
				 \param tessEvalPath a const std::string& - The tessellation evaluation shader file path */

		virtual void setUniforms(const std::vector<std::string>& uniformNames) = 0; //!< Set all uniform locations
			/*!< \param uniformNames a const std::vector<std::string>& - All uniform names */

		virtual void start() = 0; //!< Start the shader
		virtual void stop() = 0; //!< Stop using the shader
		virtual void destroy() = 0; //!< Destroy the shader

		virtual inline const uint32_t getID() const { return m_programID; } //!< Get the program ID
			/*!< \return a const uint32_t - The program ID */

		virtual void sendInt(const std::string& uniformName, const int value) = 0; //!< Upload integer to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param value a const int - The uniform value */
		virtual void sendIntArray(const std::string& uniformName, int32_t* values, const uint32_t count) = 0; //!< Upload an array of integers to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param values a int32_t* - The uniform values
				 \param count a const uint32_t - The number of elements */
		virtual void sendBool(const std::string& uniformName, const bool value) = 0; //!< Upload bool to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param value a const bool - The uniform value */
		virtual void sendFloat(const std::string& uniformName, const float value) = 0; //!< Upload float to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param value a const float - The uniform value */
		virtual void sendVec2(const std::string& uniformName, const glm::vec2& value) = 0; //!< Upload vec2 to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param value a const glm::vec2& - The uniform value */
		virtual void sendVec3(const std::string& uniformName, const glm::vec3& value) = 0; //!< Upload vec3 to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param value a const glm::vec3& - The uniform value */
		virtual void sendVec4(const std::string& uniformName, const glm::vec4& value) = 0; //!< Upload vec4 to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param value a const glm::vec4& - The uniform value */
		virtual void sendMat3(const std::string& uniformName, const glm::mat3& mat) = 0; //!< Upload mat3 to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param mat a const glm::mat3& - The uniform value */
		virtual void sendMat4(const std::string& uniformName, const glm::mat4& mat) = 0; //!< Upload mat4 to shader
			/*!< \param uniformName a const std::string& - The uniform name
				 \param mat a const glm::mat4& - The uniform value */

		static ShaderProgram* create(); //!< Create a shader program
	};
}
#endif