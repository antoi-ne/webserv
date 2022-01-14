#include "Parser.hpp"
#include "utils/utils.hpp"
#include "utils/Log.hpp"

namespace ws
{
	namespace conf
	{
		static std::vector<std::string>::iterator parse_server(std::vector<std::string> content, std::vector<std::string>::iterator it, Server & server)
		{
			std::string tab("\t");
			while (it != content.end())
			{
				if (it->length() == 0) // empty line
				{}
				else if (!it->compare(0, tab.size(), tab)) // starts with tab
				{
					std::string label = it->substr(0, it->find(" "));
					if (label == "sever_name")
					{
						std::vector<std::string> names = utils::split(*it, ' ');
						if (names.size() < 2)
							utils::Log::fatal("invalid token: server_name");
						names.erase(names.begin());
						server.server_names = names;
					}
				}
				else // does not start with tab
					break;
				it++;
			}
			return it;
		}

		Parser::Parser(std::string path)
		{
			std::ifstream ifs;
			std::string line;
			std::vector<std::string> content;

			ifs.open(path);

			while (std::getline(ifs, line))
				content.push_back(line);
			ifs.close();

			std::vector<std::string>::iterator it = content.begin();
			while (it != content.end())
			{
				if (it->length() == 0)  // empty line
				{
					it++;
					continue;
				}
				else if (*it == "server:") // inside server block
				{
					Server server;
					std::cout << "SERVER" << std::endl;
					it = parse_server(content, ++it, server);
				}
				else // invalid line
				{
					std::cerr << "invalid config file" << std::endl;
					exit(1);
				}
			}
		}

		Parser::~Parser()
		{}
	}
}