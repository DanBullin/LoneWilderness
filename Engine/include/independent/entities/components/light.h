/*! \file light.h
*
* \brief A light component which defines a light source
*
* \author Daniel Bullin
*
*/
#ifndef LIGHT_H
#define LIGHT_H

#include "independent/entities/entityComponent.h"

namespace Engine
{
	/*! \class DirectionalLight
	* \brief A directional light object
	*/
	class DirectionalLight : public EntityComponent
	{
	private:
		glm::vec3 m_direction; //!< The direction of the light
		glm::vec3 m_ambient; //!< The ambient factor of the light
		glm::vec3 m_diffuse; //!< The diffuse factor of the light
		glm::vec3 m_specular; //!< The specular factor of the light
	public:
		DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular); //!< Constructor
		~DirectionalLight(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the light
		void printComponentDetails() override; //!< Print component details

		glm::vec3 getWorldPosition(); //!< Get the world position of the light

		void setDirection(const glm::vec3& direction); //!< Set the direction vector of the light
		glm::vec3 getDirection() const; //!< Get the light's direction
		void setAmbientFactor(const glm::vec3& ambient); //!< Set the ambient factor of the light
		glm::vec3 getAmbientFactor() const; //!< Get the ambient factor of the light
		void setDiffuseFactor(const glm::vec3& diffuse); //!< Set the diffuse factor of the light
		glm::vec3 getDiffuseFactor() const; //!< Get the diffuse factor of the light
		void setSpecularFactor(const glm::vec3& specular); //!< Set the specular factor of the light
		glm::vec3 getSpecularFactor() const; //!< Get the specular factor of the light
	};

	/*! \class PointLight
	* \brief A point light object
	*/
	class PointLight : public EntityComponent
	{
	private:
		glm::vec3 m_ambient; //!< The ambient factor of the light
		glm::vec3 m_diffuse; //!< The diffuse factor of the light
		glm::vec3 m_specular; //!< The specular factor of the light
		float m_constant; //!< The constant value used for attenuation
		float m_linear; //!< The linear value used for attenuation
		float m_quadratic; //!< The quadratic value used for attenuation
	public:
		PointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float constant, const float linear, const float quadratic); //!< Constructor
		~PointLight(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the light
		void printComponentDetails() override; //!< Print component details

		glm::vec3 getWorldPosition(); //!< Get the world position of the light

		void setAmbientFactor(const glm::vec3& ambient); //!< Set the ambient factor of the light
		glm::vec3 getAmbientFactor() const; //!< Get the ambient factor of the light
		void setDiffuseFactor(const glm::vec3& diffuse); //!< Set the diffuse factor of the light
		glm::vec3 getDiffuseFactor() const; //!< Get the diffuse factor of the light
		void setSpecularFactor(const glm::vec3& specular); //!< Set the specular factor of the light
		glm::vec3 getSpecularFactor() const; //!< Get the specular factor of the light

		void setConstant(const float constant); //!< Set the constant value used for attenuation
		float getConstant() const; //!< Get the constant value used for attenuation
		void setLinear(const float linear); //!< Set the linear value used for attenuation
		float getLinear() const; //!< Get the linear value used for attenuation
		void setQuadratic(const float quadratic); //!< Set the quadratic value used for attenuation
		float getQuadratic() const; //!< Get the quadratic value used for attenuation
	};

	/*! \class SpotLight
	* \brief A spot light object
	*/
	class SpotLight : public EntityComponent
	{
	private:
		glm::vec3 m_direction; //!< The direction of the light
		glm::vec3 m_ambient; //!< The ambient factor of the light
		glm::vec3 m_diffuse; //!< The diffuse factor of the light
		glm::vec3 m_specular; //!< The specular factor of the light
		float m_constant; //!< The constant value used for attenuation
		float m_linear; //!< The linear value used for attenuation
		float m_quadratic; //!< The quadratic value used for attenuation
		float m_cutOff; //!< The inner cutoff of the spotlight
		float m_outerCutOff; //!< The outer cutoff of the spotlight
	public:
		SpotLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic); //!< Constructor
		~SpotLight(); //!< Destructor

		void onAttach() override; //!< Occurs when component is attached to an entity
		void onDetach() override; //!< Occurs when component is detached from an entity
		void onUpdate(const float timestep, const float totalTime) override; //!< Update the light
		void printComponentDetails() override; //!< Print component details

		glm::vec3 getWorldPosition(); //!< Get the world position of the light

		void setDirection(const glm::vec3& direction); //!< Set the direction vector of the light
		glm::vec3 getDirection() const; //!< Get the light's direction

		void setAmbientFactor(const glm::vec3& ambient); //!< Set the ambient factor of the light
		glm::vec3 getAmbientFactor() const; //!< Get the ambient factor of the light
		void setDiffuseFactor(const glm::vec3& diffuse); //!< Set the diffuse factor of the light
		glm::vec3 getDiffuseFactor() const; //!< Get the diffuse factor of the light
		void setSpecularFactor(const glm::vec3& specular); //!< Set the specular factor of the light
		glm::vec3 getSpecularFactor() const; //!< Get the specular factor of the light

		void setConstant(const float constant); //!< Set the constant value used for attenuation
		float getConstant() const; //!< Get the constant value used for attenuation
		void setLinear(const float linear); //!< Set the linear value used for attenuation
		float getLinear() const; //!< Get the linear value used for attenuation
		void setQuadratic(const float quadratic); //!< Set the quadratic value used for attenuation
		float getQuadratic() const; //!< Get the quadratic value used for attenuation

		void setCutOff(const float linear); //!< Set the cutoff value
		float getCutOff() const; //!< Get the cutoff value
		void setOuterCutOff(const float quadratic); //!< Set the outer cutoff value
		float getOuterCutOff() const; //!< Get the outer cutoff value
	};
}
#endif