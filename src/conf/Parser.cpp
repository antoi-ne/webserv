#include "Parser.hpp"

namespace ws
{
	namespace conf
	{
		Parser::Parser(std::string file_path)
		{
			std::ifstream   fd(file_path);
    		try {
				std::string     line;
				while (std::getline(fd, line) != NULL)
				{
					std::cout << line << std::endl;
					if (line == "server:")
					{
						mapping_servers(this->config.servers, fd);
					}
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
			return(this->config);
		}
	}
}
