/*! \file nameUtils.cpp
*
* \brief A name utility class which performs various helpful functions in regards to names/strings.
*
* \author Joshua Simons
*
*/

#include "independent/utils/nameUtils.h"

namespace Engine
{
	//! rename()
	/*!
	\param name a const std::string& - The name to rename
	\return a std::string - The new name
	*/
	std::string NameUtils::rename(const std::string& name)
	{
		enum state {starting, reading, outing};
		state status = starting;
		//bool reading = false;
		//bool outing = false;
		std::string result;
		int _iNumber = 0;
		int tens = 1;

		// There is a 
		for (auto it = name.rbegin(); it != name.rend(); it++) {
			switch (status)
			{
			case starting: //Beginning
				if (*it == m_end) {
					//correct character detected, time to start reading the number
					status = reading;
					break;
				}
				else {
					//incorrect character detected, need to exit loop and keep name as is
					result = name; 
					it = name.rend() - 1; //Breaks us out of for loop
					break;
				}
				break;

			case reading: //Reading the number part
				if (*it == m_start) { //character is the start character, we can stop checking the number
					status = outing;
					continue;
				}
				else if (*it < 48 || *it > 57) { //character ISNT a number, need to exit loop and keep name as is
					_iNumber = 0;
					result = name;
					it = name.rend() - 1; //Breaks us out of for loop
					break;
				}
				else { //character is a number, we make sure its got the right tens then add it to our existing number
					_iNumber += ((int)*it - 48) * tens;
					tens *= 10; //make tens ten times bigger so its ready for the next number
				}
				break;

			case outing: //ouputting the string before the number straight to the result
				result = *it + result;
				break;
			}
		}
		_iNumber++;
		// name and 999 --> name(999)
		result = result + m_start + std::to_string(_iNumber) + m_end;

		return result;
	}
}