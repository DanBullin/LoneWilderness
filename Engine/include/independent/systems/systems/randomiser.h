/*! \file randomiser.h 
*
* \brief A random number generator class which supports different distributions
*
* \author DMU Course material
*
*/
#ifndef RANDOMISER_H
#define RANDOMISER_H

#include <random>
#include "independent/systems/system.h"

namespace Engine
{
	using UniIntDistrib = std::uniform_int_distribution<int32_t>; //!< Type alias because its quite a handful
	using UniRealDistrib = std::uniform_real_distribution<float>; //!< Type alias because its quite a handful

	/*! \class Randomiser
	* \brief A random number generator class
	*/
	class Randomiser : public System {
	private:
		static bool s_enabled; //!< Is this randomiser system enabled
		static std::shared_ptr<std::mt19937> s_randomGenerator; //!< Random number generator
		static std::uniform_int_distribution<int32_t> s_uniformInt; //!< Uniform integer distribution
		static std::uniform_real_distribution<float> s_uniformFloat; //!< Uniform float distribution
		static float s_intRange; //!< Range of int32_t
		static float s_floatRange; //!< Range of 32bit float
	public:
		Randomiser(); //!< Constructor
		~Randomiser(); //!< Destructor
		void start() override; //!< Start the system
		void stop() override; //!< Stop the system

		static int32_t uniformIntBetween(const int32_t lower, const int32_t upper); //!< Get an integer i such that lower <= i <= upper
		static float uniformFloatBetween(const float lower, const float upper); //!< Get an float i such that lower <= i <= upper
		static int32_t normalInt(const float c, const float sigma); //!< Get an int from the Gaussian distribution described by C and Sigma
		static float normalFloat(const float c, const float sigma); //!< Get a float from the Gaussian distribution described by C and Sigma
	};
}
#endif