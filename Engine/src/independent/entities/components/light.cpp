/*! \file light.cpp
*
* \brief A light component which defines a light source
*
* \author Daniel Bullin
*
*/
#include "independent/entities/components/light.h"
#include "independent/systems/systems/log.h"
#include "independent/entities/entity.h"

namespace Engine
{

#pragma region "Direction"

	//! DirectionalLight()
	/*
	\param direction a const glm::vec3& - The light's direction
	\param ambient a const glm::vec3& - The light's ambient factor
	\param diffuse a const glm::vec3& - The light's diffuse factor
	\param specular a const glm::vec3& - The light's specular factor
	*/
	DirectionalLight::DirectionalLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
		: EntityComponent(ComponentType::DirectionalLight), m_direction(direction), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular)
	{
	}

	//! ~DirectionalLight()
	DirectionalLight::~DirectionalLight()
	{
	}

	//! onAttach()
	void DirectionalLight::onAttach()
	{
	}

	//! onDetach
	void DirectionalLight::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void DirectionalLight::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void DirectionalLight::printComponentDetails()
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[DirectionalLight::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("================================================");
		ENGINE_TRACE("Light Details for light attached to Entity: {0}", parent->getName());
		ENGINE_TRACE("================================================");
		ENGINE_TRACE("World Position: {0}, {1}, {2}.", getWorldPosition().x, getWorldPosition().y, getWorldPosition().z);
		ENGINE_TRACE("Direction: {0}, {1}, {2}.", m_direction.x, m_direction.y, m_direction.z);
		ENGINE_TRACE("Ambient Factor: {0}, {1}, {2}.", m_ambient.x, m_ambient.y, m_ambient.z);
		ENGINE_TRACE("Diffuse Factor: {0}, {1}, {2}.", m_diffuse.x, m_diffuse.y, m_diffuse.z);
		ENGINE_TRACE("Specular Factor: {0}, {1}, {2}.", m_specular.x, m_specular.y, m_specular.z);
		ENGINE_TRACE("================================================");
	}

	//! setDirection()
	/*
	\param direction a const glm::vec3& - The desired light direction
	*/
	void DirectionalLight::setDirection(const glm::vec3& direction)
	{
		m_direction = direction;
	}

	//! getDirection()
	/*
	\return a glm::vec3 - The light direction
	*/
	glm::vec3 DirectionalLight::getDirection() const
	{
		return m_direction;
	}

	//! getWorldPosition()
	/*
	\return a glm::vec3 - The world position
	*/
	glm::vec3 DirectionalLight::getWorldPosition()
	{
		Entity* parent = getParent();

		if (parent)
		{
			Transform* trans = parent->getComponent<Transform>();

			if (trans)
				return trans->getPosition();
			else
				ENGINE_ERROR("[DirectionalLight::getWorldPosition] The entity this component is attached to does not have a valid transform. Entity: {0}.", parent->getName());
		}
		else
			ENGINE_ERROR("[DirectionalLight::getWorldPosition] The entity this component is attached is not valid.");

		return glm::vec3(0.f, 0.f, 0.f);
	}

	//! setAmbientFactor()
	/*
	\param ambient a const glm::vec3& - The desired ambient factor
	*/
	void DirectionalLight::setAmbientFactor(const glm::vec3& ambient)
	{
		m_ambient = ambient;
	}

	//! getAmbientFactor()
	/*
	\return a glm::vec3 - The ambient factor
	*/
	glm::vec3 DirectionalLight::getAmbientFactor() const
	{
		return m_ambient;
	}

	//! setDiffuseFactor()
	/*
	\param diffuse a const glm::vec3& - The desired diffuse factor
	*/
	void DirectionalLight::setDiffuseFactor(const glm::vec3& diffuse)
	{
		m_diffuse = diffuse;
	}

	//! getDiffuseFactor()
	/*
	\return a glm::vec3 - The diffuse factor
	*/
	glm::vec3 DirectionalLight::getDiffuseFactor() const
	{
		return m_diffuse;
	}

	//! setSpecularFactor()
	/*
	\param specular a const glm::vec3& - The desired specular factor
	*/
	void DirectionalLight::setSpecularFactor(const glm::vec3& specular)
	{
		m_specular = specular;
	}

	//! getSpecularFactor()
	/*
	\return a glm::vec3 - The specular factor
	*/
	glm::vec3 DirectionalLight::getSpecularFactor() const
	{
		return m_specular;
	}

#pragma endregion

#pragma region "Point"

	//! PointLight()
	/*
	\param ambient a const glm::vec3& - The light's ambient factor
	\param diffuse a const glm::vec3& - The light's diffuse factor
	\param specular a const glm::vec3& - The light's specular factor
	\param constant a const float - The constant value used for attenuation
	\param linear a const float - The linear value used for attenuation
	\param quadratic a const float - The quadratic value used for attenuation
	*/
	PointLight::PointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float constant, const float linear, const float quadratic)
		: EntityComponent(ComponentType::PointLight), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_constant(constant), m_linear(linear), m_quadratic(quadratic)
	{
	}

	//! ~PointLight()
	PointLight::~PointLight()
	{
	}

	//! onAttach()
	void PointLight::onAttach()
	{
	}

	//! onDetach
	void PointLight::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void PointLight::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void PointLight::printComponentDetails()
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[PointLight::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("================================================");
		ENGINE_TRACE("Light Details for light attached to Entity: {0}", parent->getName());
		ENGINE_TRACE("================================================");
		ENGINE_TRACE("World Position: {0}, {1}, {2}.", getWorldPosition().x, getWorldPosition().y, getWorldPosition().z);
		ENGINE_TRACE("Ambient Factor: {0}, {1}, {2}.", m_ambient.x, m_ambient.y, m_ambient.z);
		ENGINE_TRACE("Diffuse Factor: {0}, {1}, {2}.", m_diffuse.x, m_diffuse.y, m_diffuse.z);
		ENGINE_TRACE("Specular Factor: {0}, {1}, {2}.", m_specular.x, m_specular.y, m_specular.z);
		ENGINE_TRACE("Constant Value: {0}.", m_constant);
		ENGINE_TRACE("Linear Value: {0}.", m_linear);
		ENGINE_TRACE("Quadratic Value: {0}.", m_quadratic);
		ENGINE_TRACE("================================================");
	}

	//! getWorldPosition()
	/*
	\return a glm::vec3 - The world position
	*/
	glm::vec3 PointLight::getWorldPosition()
	{
		Entity* parent = getParent();

		if (parent)
		{
			Transform* trans = parent->getComponent<Transform>();

			if (trans)
				return trans->getPosition();
			else
				ENGINE_ERROR("[PointLight::getWorldPosition] The entity this component is attached to does not have a valid transform. Entity: {0}.", parent->getName());
		}
		else
			ENGINE_ERROR("[PointLight::getWorldPosition] The entity this component is attached is not valid.");

		return glm::vec3(0.f, 0.f, 0.f);
	}

	//! setAmbientFactor()
	/*
	\param ambient a const glm::vec3& - The desired ambient factor
	*/
	void PointLight::setAmbientFactor(const glm::vec3& ambient)
	{
		m_ambient = ambient;
	}

	//! getAmbientFactor()
	/*
	\return a glm::vec3 - The ambient factor
	*/
	glm::vec3 PointLight::getAmbientFactor() const
	{
		return m_ambient;
	}

	//! setDiffuseFactor()
	/*
	\param diffuse a const glm::vec3& - The desired diffuse factor
	*/
	void PointLight::setDiffuseFactor(const glm::vec3& diffuse)
	{
		m_diffuse = diffuse;
	}

	//! getDiffuseFactor()
	/*
	\return a glm::vec3 - The diffuse factor
	*/
	glm::vec3 PointLight::getDiffuseFactor() const
	{
		return m_diffuse;
	}

	//! setSpecularFactor()
	/*
	\param specular a const glm::vec3& - The desired specular factor
	*/
	void PointLight::setSpecularFactor(const glm::vec3& specular)
	{
		m_specular = specular;
	}

	//! getSpecularFactor()
	/*
	\return a glm::vec3 - The specular factor
	*/
	glm::vec3 PointLight::getSpecularFactor() const
	{
		return m_specular;
	}

	//! setConstant()
	/*
	\param constant a const float - The constant value for the attenuation calculation
	*/
	void PointLight::setConstant(const float constant)
	{
		m_constant = constant;
	}

	//! getConstant
	/*
	\return a float - The constant value used for the attenuation calculation
	*/
	float PointLight::getConstant() const
	{
		return m_constant;
	}

	//! setLinear()
	/*
	\param linear a const float - The linear value for the attenuation calculation
	*/
	void PointLight::setLinear(const float linear)
	{
		m_linear = linear;
	}

	//! getLinear
	/*
	\return a float - The linear value used for the attenuation calculation
	*/
	float PointLight::getLinear() const
	{
		return m_linear;
	}

	//! setQuadratic()
	/*
	\param quadratic a const float - The quadratic value for the attenuation calculation
	*/
	void PointLight::setQuadratic(const float quadratic)
	{
		m_quadratic = quadratic;
	}

	//! getQuadratic
	/*
	\return a float - The quadratic value used for the attenuation calculation
	*/
	float PointLight::getQuadratic() const
	{
		return m_quadratic;
	}

#pragma endregion

#pragma region "Spot"

	//! SpotLight()
	/*
	\param direction a const glm::vec3& - The light's direction
	\param ambient a const glm::vec3& - The light's ambient factor
	\param diffuse a const glm::vec3& - The light's diffuse factor
	\param specular a const glm::vec3& - The light's specular factor
	\param cutOff a const float - The cutoff value
	\param outerCutOff a const float - The outerCutOff value
	\param constant a const float - The constant value used for attenuation
	\param linear a const float - The linear value used for attenuation
	\param quadratic a const float - The quadratic value used for attenuation
	*/
	SpotLight::SpotLight(const glm::vec3& direction, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const float cutOff, const float outerCutOff, const float constant, const float linear, const float quadratic)
		: EntityComponent(ComponentType::SpotLight), m_direction(direction), m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_cutOff(cutOff), m_outerCutOff(outerCutOff), m_constant(constant), m_linear(linear), m_quadratic(quadratic)
	{
	}

	//! ~SpotLight()
	SpotLight::~SpotLight()
	{
	}

	//! onAttach()
	void SpotLight::onAttach()
	{
	}

	//! onDetach
	void SpotLight::onDetach()
	{
	}

	//! onUpdate()
	/*!
	\param timestep a const float - The time step
	\param totalTime a const float - The total time of the application
	*/
	void SpotLight::onUpdate(const float timestep, const float totalTime)
	{
	}

	//! printComponentDetails()
	void SpotLight::printComponentDetails()
	{
		Entity* parent = getParent();

		if (!parent)
		{
			ENGINE_ERROR("[SpotLight::printComponentDetails] This component does not have a valid parent entity. Component Name: {0}.", m_name);
			return;
		}

		ENGINE_TRACE("================================================");
		ENGINE_TRACE("Light Details for light attached to Entity: {0}", getParent()->getName());
		ENGINE_TRACE("================================================");
		ENGINE_TRACE("World Position: {0}, {1}, {2}.", getWorldPosition().x, getWorldPosition().y, getWorldPosition().z);
		ENGINE_TRACE("Direction: {0}, {1}, {2}.", m_direction.x, m_direction.y, m_direction.z);
		ENGINE_TRACE("Ambient Factor: {0}, {1}, {2}.", m_ambient.x, m_ambient.y, m_ambient.z);
		ENGINE_TRACE("Diffuse Factor: {0}, {1}, {2}.", m_diffuse.x, m_diffuse.y, m_diffuse.z);
		ENGINE_TRACE("Specular Factor: {0}, {1}, {2}.", m_specular.x, m_specular.y, m_specular.z);
		ENGINE_TRACE("Constant Value: {0}.", m_constant);
		ENGINE_TRACE("Linear Value: {0}.", m_linear);
		ENGINE_TRACE("Quadratic Value: {0}.", m_quadratic);
		ENGINE_TRACE("Cut off Value: {0}.", m_cutOff);
		ENGINE_TRACE("Outer cuttoff Value: {0}.", m_outerCutOff);
		ENGINE_TRACE("================================================");
	}

	//! getWorldPosition()
	/*
	\return a glm::vec3 - The world position
	*/
	glm::vec3 SpotLight::getWorldPosition()
	{
		Entity* parent = getParent();

		if (parent)
		{
			Transform* trans = parent->getComponent<Transform>();

			if (trans)
				return trans->getPosition();
			else
				ENGINE_ERROR("[SpotLight::getWorldPosition] The entity this component is attached to does not have a valid transform. Entity: {0}.", parent->getName());
		}
		else
			ENGINE_ERROR("[SpotLight::getWorldPosition] The entity this component is attached is not valid.");

		return glm::vec3(0.f, 0.f, 0.f);
	}

	//! setDirection()
	/*
	\param direction a const glm::vec3& - The desired light direction
	*/
	void SpotLight::setDirection(const glm::vec3& direction)
	{
		m_direction = direction;
	}

	//! getDirection()
	/*
	\return a glm::vec3 - The light direction
	*/
	glm::vec3 SpotLight::getDirection() const
	{
		return m_direction;
	}

	//! setAmbientFactor()
	/*
	\param ambient a const glm::vec3& - The desired ambient factor
	*/
	void SpotLight::setAmbientFactor(const glm::vec3& ambient)
	{
		m_ambient = ambient;
	}

	//! getAmbientFactor()
	/*
	\return a glm::vec3 - The ambient factor
	*/
	glm::vec3 SpotLight::getAmbientFactor() const
	{
		return m_ambient;
	}

	//! setDiffuseFactor()
	/*
	\param diffuse a const glm::vec3& - The desired diffuse factor
	*/
	void SpotLight::setDiffuseFactor(const glm::vec3& diffuse)
	{
		m_diffuse = diffuse;
	}

	//! getDiffuseFactor()
	/*
	\return a glm::vec3 - The diffuse factor
	*/
	glm::vec3 SpotLight::getDiffuseFactor() const
	{
		return m_diffuse;
	}

	//! setSpecularFactor()
	/*
	\param specular a const glm::vec3& - The desired specular factor
	*/
	void SpotLight::setSpecularFactor(const glm::vec3& specular)
	{
		m_specular = specular;
	}

	//! getSpecularFactor()
	/*
	\return a glm::vec3 - The specular factor
	*/
	glm::vec3 SpotLight::getSpecularFactor() const
	{
		return m_specular;
	}

	//! setCutOff()
	/*
	\param cutOff a const float - The cut off value desired
	*/
	void SpotLight::setCutOff(const float cutOff)
	{
		if(cutOff >= 0)
			m_cutOff = cutOff;
		else
			ENGINE_ERROR("[SpotLight::setCutOff] An invalid value for cutoff was provided. Value: {0}.", cutOff);
	}

	//! getCutOff()
	/*
	\return a float - The cut off value
	*/
	float SpotLight::getCutOff() const
	{
		return m_cutOff;
	}

	//! setOuterCutOff()
	/*
	\param outerCutOff a const float - The outer cut off value desired
	*/
	void SpotLight::setOuterCutOff(const float outerCutOff)
	{
		if (outerCutOff >= 0)
			m_outerCutOff = outerCutOff;
		else
			ENGINE_ERROR("[SpotLight::setOuterCutOff] An invalid value for outer cutoff was provided. Value: {0}.", outerCutOff);
	}

	//! getOuterCutOff()
	/*
	\return a float - The outer cut off value
	*/
	float SpotLight::getOuterCutOff() const
	{
		return m_outerCutOff;
	}

	//! setConstant()
	/*
	\param constant a const float - The constant value for the attenuation calculation
	*/
	void SpotLight::setConstant(const float constant)
	{
		m_constant = constant;
	}

	//! getConstant
	/*
	\return a float - The constant value used for the attenuation calculation
	*/
	float SpotLight::getConstant() const
	{
		return m_constant;
	}

	//! setLinear()
	/*
	\param linear a const float - The linear value for the attenuation calculation
	*/
	void SpotLight::setLinear(const float linear)
	{
		m_linear = linear;
	}

	//! getLinear
	/*
	\return a float - The linear value used for the attenuation calculation
	*/
	float SpotLight::getLinear() const
	{
		return m_linear;
	}

	//! setQuadratic()
	/*
	\param quadratic a const float - The quadratic value for the attenuation calculation
	*/
	void SpotLight::setQuadratic(const float quadratic)
	{
		m_quadratic = quadratic;
	}

	//! getQuadratic
	/*
	\return a float - The quadratic value used for the attenuation calculation
	*/
	float SpotLight::getQuadratic() const
	{
		return m_quadratic;
	}

#pragma endregion

}