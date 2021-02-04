/*! \file GLFWCodes.h 
*
* \brief Input codes in GLFW's format
*
* \author Daniel Bullin
*
*/
#ifndef GLFWCODES_H
#define GLFWCODES_H

namespace Engine
{
	namespace Keys
	{
		/*! \enum Keys
		* \brief Enum containing key values in GLFW
		*/
		enum Keys
		{
			SPACE = 32,
			APOSTROPHE = 39,
			COMMA = 44,
			MINUS = 45,
			PERIOD = 46,
			FORWARDSLASH = 47,
			NUMBER0 = 48,
			NUMBER1 = 49,
			NUMBER2 = 50,
			NUMBER3 = 51,
			NUMBER4 = 52,
			NUMBER5 = 53,
			NUMBER6 = 54,
			NUMBER7 = 55,
			NUMBER8 = 56,
			NUMBER9 = 57,
			SEMICOLON = 59,
			EQUALS = 61,
			A = 65,
			B = 66,
			C = 67,
			D = 68,
			E = 69,
			F = 70,
			G = 71,
			H = 72,
			I = 73,
			J = 74,
			K = 75,
			L = 76,
			M = 77,
			N = 78,
			O = 79,
			P = 80,
			Q = 81,
			R = 82,
			S = 83,
			T = 84,
			U = 85,
			V = 86,
			W = 87,
			X = 88,
			Y = 89,
			Z = 90,
			LEFT_SQUARE_BRACKET = 91,
			BACKSLASH = 92,
			RIGHT_SQUARE_BRACKET = 93,
			GRACE_ACCENT = 96,
			ESC = 256,
			ENTER = 257,
			TAB = 258,
			BACKSPACE = 259,
			RIGHT = 262,
			LEFT = 263,
			DOWN = 264,
			UP = 265,
			CAPS_LOCK = 280,
			SCROLL_LOCK = 281,
			F1 = 290,
			F2 = 291,
			F3 = 292,
			F4 = 293,
			F5 = 294,
			F6 = 295,
			F7 = 296,
			F8 = 297,
			F9 = 298,
			F10 = 299,
			F11 = 300,
			F12 = 301,
			DIVIDE = 331,
			MULTIPLY = 332,
			LEFT_SHIFT = 340,
			LEFT_CONTROL = 341,
			LEFT_ALT = 342
		};
	}

	namespace Mouse
	{
		/*! \enum MouseButton
		* \brief Enum containing mouse button values in GLFW
		*/
		enum MouseButton
		{
			LEFTBUTTON = 0,
			RIGHTBUTTON = 1,
			MIDDLEBUTTON = 2,
		};
	}

	namespace InputState
	{
		/*! \enum InputState
		* \brief Enum containing input state values in GLFW
		*/
		enum InputState
		{
			RELEASE = 0,
			PRESS = 1,
			REPEAT = 2,
		};
	}
}

#endif