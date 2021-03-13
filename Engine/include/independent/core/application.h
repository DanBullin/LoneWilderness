/*! \file application.h
*
* \brief Main application, the start point of the engine
*
* \author Daniel Bullin
*
*/
#ifndef APPLICATION_H
#define APPLICATION_H

namespace Engine 
{
	/*! \class Application
	* \brief Fundemental class of Engine. A singleton which runs the game loop infinitely. Contains all the systems.
	*/
	class Application
	{
	private:
		static Application* s_instance; //!< Singleton instance of the application
		static bool s_running; //!< Is the application running? Default value of true
	protected:
		Application(); //!< Default Constructor
	public:
		virtual ~Application(); //!< Deconstructor
		void run(); //!< Main loop
		static void stop(); //!< Stop the whole application

		const bool checkExitConditions(); //!< Get all application exit conditions

		inline static Application& getInstance() { return *s_instance; } //!< Instance getter from singleton pattern
			/*!< \return an Application& - A reference to the application instance */
	};

	Application* startApplication(); //!< Function definition which provides an entry hook
}
#endif