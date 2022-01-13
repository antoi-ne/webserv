#include "Log.hpp"

namespace ws
{
	namespace utils
	{
		void Log::info(std::string message)
		{
			std::cerr << "info: " << message << std::endl;
		}

		void Log::error(std::string message)
		{
			std::cerr << "error: " << message << std::endl;
		}

		void Log::fatal(std::string message)
		{
			std::cerr << "fatal: " << message << std::endl;
			std::exit(EXIT_FAILURE);
		}
	}
}
