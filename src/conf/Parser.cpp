#include "Parser.hpp"

namespace ws
{
	namespace conf
	{
		Parser::Parser(std::string file_path)
		{
			Config config;
			std::ifstream   fd(file_path);
    		try {
				std::string     line;
				while (std::getline(fd, line) != NULL)
				{
					if (line == "server:")
						config.servers.push_back(server_attr(fd));
				}
			}
			catch(int oui){
				std::cout << "is :" << file_path << " a correct config file ?";
				}
		}

		Parser::~Parser()
		{
			// [...]
		}

		Config Parser::to_config()
		{
		}
	}
}
