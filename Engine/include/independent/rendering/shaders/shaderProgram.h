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
#include "independent/systems/components/resource.h"
#include "independent/rendering/geometry/vertexArray.h"
#include "independent/rendering/uniformBuffer.h"

namespace Engine
{
	/*! \class ShaderProgram
	* \brief An API agnostic shader program
	*/
	class ShaderProgram : public Resource
	{
	protected:
		uint32_t m_programID; //!< The shader program ID
		uint32_t m_order; //!< The order of the shader list in the renderer queue (Blending demands order importance)
		std::string m_vertexPath; //!< The file path of the vertex shader
		std::string m_fragmentPath; //!< The file path of the fragment shader
		std::string m_geometryPath; //!< The file path of the geometry shader
		std::string m_tessellationControlPath; //!< The file path of the tessellation control shader
		std::string m_tessellationEvalPath; //!< The file path of the tessellation evaluation shader
		std::map<std::string, int> m_uniforms; //!< All the uniforms for the shader program
		VertexArray* m_vertexArray; //!< The vertex data format which complies with the data format in the shader
		std::unordered_map<std::string, UniformBuffer*> m_uniformBuffers; //!< The uniform buffers used by this shader program
	public:
		static ShaderProgram* create(const std::string& shaderName); //!< Create a shader program

		ShaderProgram(const std::string& shaderName); //!< Constructor
		virtual ~ShaderProgram(); //!< Destructor

		virtual void build(VertexArray* vertexArray, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, const std::string& tessControlPath, const std::string& tessEvalPath) = 0; //!< Create a shader program based on the files passed
			/*!< \param vertexArray a VertexArray* - A pointer to the vertex array
				 \param vertexPath a const std::string& - The vertex shader file path
				 \param fragmentPath a const std::string& - The fragment shader file path
				 \param geometryPath a const std::string& - The geometry shader file path
				 \param tessControlPath a const std::string& - The tessellation control shader file path
				 \param tessEvalPath a const std::string& - The tessellation evaluation shader file path */

		virtual void setUniforms(const std::vector<std::string>& uniformNames) = 0; //!< Set all uniform locations
			/*!< \param uniformNames a const std::vector<std::string>& - All uniform names */

		virtual void setUniformBuffers(const std::unordered_map<std::string, UniformBuffer*>& uniformBuffers) = 0; //!< Set the uniform buffers used by this shader
			/*!< \param uniformBuffers a const std::unordered_map<std::string, UniformBuffer*>& - All uniform buffers associated by the block name */

		virtual void start() = 0; //!< Start the shader
		virtual void stop() = 0; //!< Stop using the shader
		virtual void destroy() = 0; //!< Destroy the shader

		inline const uint32_t getID() const { return m_programID; } //!< Get the program ID
			/*!< \return a const uint32_t - The program ID */
		inline VertexArray* getVertexArray() const { return m_vertexArray; } //!< Get the vertex array associated with this shader
			/*!< \return a VertexArray* - The vertex array associated with this shader */
		inline std::map<std::string, int>& getUniforms() { return m_uniforms; } //!< Get the uniforms
			/*!< \return a std::map<std::string, int>& - The uniforms */
		inline std::unordered_map<std::string, UniformBuffer*>& getUniformBuffers() { return m_uniformBuffers; } //!< Get the uniform buffers
			/*!< \return a std::unordered_map<std::string, UniformBuffer*>& - The uniform buffers */
		inline void setOrderImportance(const uint32_t value) { m_order = value; }; //!< Set the shader's order priority
			/*!< \param value a const uint32_T - The order priority (0 is lowest) */
		inline const uint32_t getOrderImportance() const { return m_order; } //!< Get the shader's order priority
			/*!< \return a const uint32_t - The shader's order priority */

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
	};
}
#endif