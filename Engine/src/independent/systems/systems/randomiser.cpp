/*! \file randomiser.cpp
*
* \brief A random number generator class which supports different distributions
*
* \author DMU Course material
*
*/
#include <chrono>
#include "independent/systems/systems/randomiser.h"
#include "independent/systems/systems/log.h"

namespace Engine 
{
	bool Randomiser::s_enabled = false; //!< Set to false
	std::shared_ptr<std::mt19937> Randomiser::s_randomGenerator = nullptr; //!< Initialise the random generator
	UniIntDistrib Randomiser::s_uniformInt = UniIntDistrib(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max()); //!< Initialise the uniform integer distribution
	UniRealDistrib Randomiser::s_uniformFloat = UniRealDistrib(std::numeric_limits<float>::min(), std::numeric_limits<float>::max()); //!< Initialise the uniform real distribution
	float Randomiser::s_intRange = static_cast<float>(std::numeric_limits<int32_t>::max() - (float)std::numeric_limits<int32_t>::min()); //!< Initialise the integer range
	float Randomiser::s_floatRange = std::numeric_limits<float>::max() - std::numeric_limits<float>::min(); //!< Initialise the float range

	//! Randomiser
	Randomiser::Randomiser() : System(SystemType::Randomiser)
	{
	}

	//! ~Randomiser()
	Randomiser::~Randomiser()
	{
	}

	//! start()
	void Randomiser::start()
	{
		// Start system if its disabled
		if (!s_enabled)
		{
			// Reset the generator with the time since clock's epoch
			ENGINE_INFO("[Randomiser::start] Starting the randomiser system.");
			auto now = std::chrono::high_resolution_clock::now();
			s_randomGenerator.reset(new std::mt19937(static_cast<unsigned int>(now.time_since_epoch().count())));
			s_enabled = true;
		}
	}

	//! stop()
	void Randomiser::stop()
	{
		// Stop system if its enabled
		if (s_enabled)
		{
			ENGINE_INFO("[Randomiser::stop] Stopping the randomiser system.");
			s_randomGenerator.reset();
			s_randomGenerator = nullptr;
			s_enabled = false;
		}
	}

	//! uniformIntBetween()
	/*!
	\param lower a const int32_t - The lowest number in range
	\param upper a const int32_t - The highest number in range
	\return an int32_t - The generated number
	*/
	int32_t Randomiser::uniformIntBetween(const int32_t lower, const int32_t upper)
	{
		if (s_enabled && s_randomGenerator)
			return lower + static_cast<int32_t>(((static_cast<float>(s_uniformInt(*s_randomGenerator)) / s_intRange) + 0.5f) * (upper - lower));
		else
			return 0;
	}

	//! uniformFloatBetween()
	/*!
	\param lower a const float - The lowest number in range
	\param upper a const float - The highest number in range
	\return a float - The generated number
	*/
	float Randomiser::uniformFloatBetween(const float lower, const float upper)
	{
		if (s_enabled && s_randomGenerator)
			return lower + (s_uniformFloat(*s_randomGenerator) / s_floatRange) * (upper - lower);
		else
			return 0.f;
	}

	//! normalInt()
	/*!
	\param c a const float - The mean
	\param sigma a const float - Standard deviation
	\return an int32_t - The generated number
	*/
	int32_t Randomiser::normalInt(const float c, const float sigma)
	{
		if (s_enabled && s_randomGenerator)
		{
			std::normal_distribution<float> distribution(c, sigma);
			return static_cast<int32_t>(distribution(*s_randomGenerator));
		}
		else
			return 0;
	}

	//! normalFloat()
	/*!
	\param c a const float - The mean
	\param sigma a const float - Standard deviation
	\return a float - The generated number
	*/
	float Randomiser::normalFloat(const float c, const float sigma)
	{
		if (s_enabled && s_randomGenerator)
		{
			std::normal_distribution<float> distribution(c, sigma);
			return distribution(*s_randomGenerator);
		}
		else
			return 0.f;
	}
}