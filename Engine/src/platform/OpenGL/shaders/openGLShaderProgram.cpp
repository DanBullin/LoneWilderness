/*! \file openGLShaderProgram.cpp
*
* \brief An OpenGL Shader program
*
* \author Daniel Bullin
*
*/
#include <glad/glad.h>
#include "independent/systems/systems/log.h"
#include "independent/systems/systems/resourceManager.h"
#include "platform/OpenGL/shaders/openGLShaderProgram.h"

namespace Engine
{
	//! constructShaders()
	/*!
	\param shaderType a const int - The OpenGL shader type
	\param filePath a const char* - The file path to the shader to be constructed
	\return an int32_t - The shader object ID
	*/
	uint32_t OpenGLShaderProgram::constructShaders(const int shaderType, const char* filePath)
	{
		uint32_t shaderID;
		// Create shader by type
		shaderID = glCreateShader(shaderType);
		// Get the source code from file
		std::string srcCode = ResourceManager::getContents(filePath);
		// Convert to char*
		const char* srcCodeChar = srcCode.c_str();

		// Upload source and compile
		glShaderSource(shaderID, 1, &srcCodeChar, NULL);
		glCompileShader(shaderID);

		// Check compilation status
		GLint success;
		GLchar infoLog[1024];
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// Could not compile, send error log
			glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
			ENGINE_ERROR("[OpenGLShaderProgram::constructShaders] Cannot compile shader with file path: {0}, reason: {1}", filePath, infoLog);
			// Delete shader for good measure
			glDeleteShader(shaderID);
			shaderID = 0;
		}
		// Return the ID
		return shaderID;
	}

	//! OpenGLShaderProgram()
	/*
	\param shaderName a const std::string& - The name of the shader program
	*/
	OpenGLShaderProgram::OpenGLShaderProgram(const std::string& shaderName) : ShaderProgram(shaderName)
	{
	}

	//! ~OpenGLShaderProgram()
	OpenGLShaderProgram::~OpenGLShaderProgram()
	{
		ENGINE_INFO("[OpenGLShaderProgram::~OpenGLShaderProgram] Deleting shader program with ID: {0}, Name: {1}.", m_programID, m_name);
		glDeleteProgram(m_programID);

		if (m_vertexArray) m_vertexArray->decreaseCount();
		m_vertexArray = nullptr;

		for (auto& ubo : m_uniformBuffers)
			ubo.second->decreaseCount();

		m_uniformBuffers.clear();
	}

	//! build()
	/*!
	\param vertexArray a VertexArray* - A pointer to the vertex array
	\param vertexPath a const std::string& - The vertex shader file path
	\param fragmentPath a const std::string& - The fragment shader file path
	\param geometryPath a const std::string& -The geometry shader file path
	\param tessControlPath a const std::string& - The tessellation control shader file path
	\param tessEvalPath a const std::string& - The tessellation evaluation shader file path
	*/
	void OpenGLShaderProgram::build(VertexArray* vertexArray, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath, const std::string& tessControlPath, const std::string& tessEvalPath)
	{
		if (m_programID != 0) return;

		if (vertexArray) m_vertexArray = vertexArray;

		// Create the shader objects by providing path
		// buildShader creates, loads src code, compiles and returns id
		GLuint VertexID = constructShaders(GL_VERTEX_SHADER, vertexPath.c_str());
		GLuint FragmentID = constructShaders(GL_FRAGMENT_SHADER, fragmentPath.c_str());
		GLuint GeometryID;
		GLuint TessControlID;
		GLuint TessEvaluationID;
		// Only do geometry shader if file was provided
		if (geometryPath != "") { GeometryID = constructShaders(GL_GEOMETRY_SHADER, geometryPath.c_str()); }
		// Only do tess control shader if file was provided
		if (tessControlPath != "") { TessControlID = constructShaders(GL_TESS_CONTROL_SHADER, tessControlPath.c_str()); }
		// Only do tess evaluation shader if file was provided
		if (tessEvalPath != "") { TessEvaluationID = constructShaders(GL_TESS_EVALUATION_SHADER, tessEvalPath.c_str()); }

		// Create the program
		m_programID = glCreateProgram();
		// Attach shader objects, add geometry, tess control and tess eval if they exist
		glAttachShader(m_programID, VertexID);
		glAttachShader(m_programID, FragmentID);
		if (geometryPath != "") { glAttachShader(m_programID, GeometryID); }
		if (tessControlPath != "") { glAttachShader(m_programID, TessControlID); }
		if (tessEvalPath != "") { glAttachShader(m_programID, TessEvaluationID); }

		// Link
		glLinkProgram(m_programID);

		// Get link status
		GLint success;
		GLchar infoLog[1024];
		glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
		if (!success)
		{
			// Couldn't link, log error
			glGetProgramInfoLog(m_programID, 1024, NULL, infoLog);
			ENGINE_ERROR("[OpenGLShaderProgram::build] Cannot link program: {0}", infoLog);
			glDeleteProgram(m_programID);
		}
		// Shader objects no longer needed as linked with program, delete
		glDeleteShader(VertexID);
		glDeleteShader(FragmentID);
		if (geometryPath != "")
			glDeleteShader(GeometryID);

		if (tessControlPath != "")
			glDeleteShader(TessControlID);

		if (tessEvalPath != "")
			glDeleteShader(TessEvaluationID);

		m_vertexPath = vertexPath;
		m_fragmentPath = fragmentPath;
		m_geometryPath = geometryPath;
		m_tessellationControlPath = tessControlPath;
		m_tessellationEvalPath = tessEvalPath;
	}

	//! setUniforms()
	/*!
	\param uniformNames a const std::vector<std::string>& - All uniform names
	*/
	void OpenGLShaderProgram::setUniforms(const std::vector<std::string>& uniformNames)
	{
		// Get all uniform locations and store results
		for (auto& name : uniformNames)
		{
			uint32_t uniformLocation = glGetUniformLocation(m_programID, name.c_str());
			if (uniformLocation == -1)
			{
				// Cannot find location so send error message
				ENGINE_ERROR("[OpenGLShaderProgram::setUniforms] Uniform with name: {0} cannot be found in program with id: {1}", name, m_programID);
			}
			else
				m_uniforms[name] = uniformLocation;
		}
	}

	//! setUniformBuffers()
	/*!
	\param uniformBuffers a const std::unordered_map<std::string, UniformBuffer*>& - The uniform buffers associated with this shader program
	*/
	void OpenGLShaderProgram::setUniformBuffers(const std::unordered_map<std::string, UniformBuffer*>& uniformBuffers)
	{
		m_uniformBuffers = uniformBuffers;
	}

	//! start()
	void OpenGLShaderProgram::start()
	{
		glUseProgram(m_programID);
	}

	//! stop()
	void OpenGLShaderProgram::stop()
	{
		glUseProgram(0);
	}

	//! destroy()
	void OpenGLShaderProgram::destroy()
	{
		glDeleteProgram(m_programID);
		glUseProgram(0);
		m_programID = 0;
	}

	//! uploadInt()
	/*!
	\param uniformName a const std::string& - The uniform name
	\param value a const int - The uniform value
	*/
	void OpenGLShaderProgram::sendInt(const std::string& uniformName, const int value)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniform1i(m_uniforms.at(uniformName), value);
	}

	//! sendIntArray()
	/*!<
	\param uniformName a const std::string& - The uniform name
	\param values a int32_t* - The uniform values
	\param count a const uint32_t - The number of elements
	*/
	void OpenGLShaderProgram::sendIntArray(const std::string& uniformName, int32_t* values, const uint32_t count)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniform1iv(m_uniforms.at(uniformName), count, values);
	}

	//! uploadBool()
	/*!
	\param uniformName a const std::string& - The uniform name
	\param value a const bool - The uniform value
	*/
	void OpenGLShaderProgram::sendBool(const std::string& uniformName, const bool value)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniform1i(m_uniforms.at(uniformName), (int)value);
	}

	//! uploadFloat()
	/*!
	\param uniformName a const std::string& - The uniform name
	\param value a const float - The uniform value
	*/
	void OpenGLShaderProgram::sendFloat(const std::string& uniformName, const float value)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniform1f(m_uniforms.at(uniformName), value);
	}

	//! uploadVec2()
	/*!
	\param uniformName a const std::string& - The uniform name
	\param value a const glm::vec2& - The uniform value
	*/
	void OpenGLShaderProgram::sendVec2(const std::string& uniformName, const glm::vec2& value)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniform2fv(m_uniforms.at(uniformName), 1, &value[0]);
	}

	//! uploadVec3()
	/*!
	\param uniformName a const std::string& - The uniform name
	\param value a const glm::vec3& - The uniform value
	*/
	void OpenGLShaderProgram::sendVec3(const std::string& uniformName, const glm::vec3& value)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniform3fv(m_uniforms.at(uniformName), 1, &value[0]);
	}

	//! uploadVec4()
	/*!
	\param uniformName a const std::string& - The uniform name
	\param value a const glm::vec4& - The uniform value
	*/
	void OpenGLShaderProgram::sendVec4(const std::string& uniformName, const glm::vec4& value)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniform4fv(m_uniforms.at(uniformName), 1, &value[0]);
	}

	//! uploadMat3()
	/*!
	\param uniformName a const std::string& - The uniform name
	\param mat a const glm::mat3& - The uniform value
	*/
	void OpenGLShaderProgram::sendMat3(const std::string& uniformName, const glm::mat3& mat)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniformMatrix3fv(m_uniforms.at(uniformName), 1, GL_FALSE, &mat[0][0]);
	}

	//! uploadMat4()
	/*!
	\param uniformName a const std::string& - The uniform name
	\param mat a const glm::mat4& - The uniform value
	*/
	void OpenGLShaderProgram::sendMat4(const std::string& uniformName, const glm::mat4& mat)
	{
		if (m_uniforms.find(uniformName) != m_uniforms.end())
			glUniformMatrix4fv(m_uniforms.at(uniformName), 1, GL_FALSE, &mat[0][0]);
	}
}