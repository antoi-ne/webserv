#include "Log.hpp"

namespace ws
{
	namespace utils
	{
		void Log::info(std::string message)
		{
			std::cout << "info: " << message << std::endl;
		}

		void Log::error(std::string message)
		{
			std::cout << "error: " << message << std::endl;
		}

		void Log::fatal(std::string message)
		{
			std::cout << "fatal: " << message << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}
}
