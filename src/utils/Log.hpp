#ifndef WS_UTILS_LOG_HPP
#define WS_UTILS_LOG_HPP

#include <string>
#include <iostream>
#include <cstdlib>

namespace ws
{
	namespace utils
	{
		class Log
		{
		public:
			static void info(std::string message);

			static void error(std::string message);

			static void fatal(std::string message);
		};
	}
}

#endif
