#include "Parser.hpp"

namespace ws
{
	namespace conf
	{

		void Parser::set_default_parameter(server_map &server){
			for (server_map::iterator it = server.begin(); it != server.end(); it++)
			{
				for (std::vector<Server>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
				{
					sit->accepted_methods.push_back(GET);
					for (std::vector<Location>::iterator it3 = sit->locations.begin(); it3 != sit->locations.end(); it3++)
					{
						if (it3->error_pages.empty())
                			it3->error_pages = sit->error_pages;
            			if (it3->root.empty())
             	   			it3->root = sit->root;
            			if (it3->max_body_size == -1)
                			it3->max_body_size = sit->max_body_size;
						if (it3->index.empty())
							it3->index = sit->index;
					}
				}
			}
		}

		void Parser::print_config(server_map server){
			for (server_map::iterator it = server.begin(); it != server.end(); it++)
			{
				for (std::vector<Server>::iterator sit = it->second.begin(); sit != it->second.end(); ++sit)
				{
					std::cout << "_____SERVER_____" << std::endl;
					std::cout << "address:"<< it->first.first << std::endl;
					std::cout << "port:" << it->first.second << std::endl;
					std::cout << "root:" << sit->root << std::endl;
					std::cout << "server_names:" << std::endl;
					for(std::vector<std::string>::iterator it3 = sit->server_names.begin(); it3 != sit->server_names.end(); it3++)
						std::cout << "-" << *it3 <<std::endl;
					std::cout << "index:" << sit->index << std::endl;
					std::cout << "accepted_method: ";
					for(std::vector<e_method>::iterator it3 = sit->accepted_methods.begin() ; it3 != sit->accepted_methods.end() ; it3++)
					{
						if (*it3 == GET)
							std::cout << " " << "GET" << " ";
						if (*it3 == POST)
							std::cout << " " << "POST" << " ";
						if (*it3 == PUT)
							std::cout << " " << "PUT" << " ";
						if (*it3 == DELETE)
							std::cout << " " << "DELETE" << " ";
					}		
					std::cout << std::endl;
					std::cout << "Error_pages:" << std::endl;
					for (ErrorPages::iterator it3 = sit->error_pages.begin() ; it3 != sit->error_pages.end(); it3++)
					{
						std::cout  << it3->first << "-->";
						std::cout << it3->second << std::endl;
					}
					std::cout << "mx_bdy_size: " << sit->max_body_size << std::endl;
					std::cout << "uplaod_path: " << sit->upload_path << std::endl;
					std::cout << "return_path: " << sit->return_path << std::endl;
					std::cout << "return_code: " << sit->return_code << std::endl;
					for (std::vector<Location>::iterator it3 = sit->locations.begin(); it3 != sit->locations.end(); it3++)
					{
						std::cout << "LOCATION:" << std::endl;
						std::cout << " ->route : " << it3->route << std::endl;
						std::cout << " ->root:" << it3->root << std::endl;
						std::cout << " ->mx_bdy_size: " << it3->max_body_size << std::endl;
						std::cout << " ->autoindex:" << it3->autoindex << std::endl;
						std::cout << " ->upload_path:" << it3->upload_path << std::endl;
						std::cout << " ->index:" << it3->index << std::endl;
						std::cout << " ->cgi_ext: " << it3->cgi_ext << std::endl;
						std::cout << " ->cgi_pass: " << it3->cgi_script << std::endl;
						std::cout << " ->return_code: " << it3->return_code << std::endl;
						std::cout << " ->return_path: " << it3->return_path << std::endl;
						std::cout << " ->accepted_method:";
						for(std::vector<e_method>::iterator it4 = it3->accepted_methods.begin() ; it4 != it3->accepted_methods.end() ; it4++)
						{
							if (*it4 == GET)
								std::cout << " " << "GET" << " ";
							if (*it4 == POST)
								std::cout << " " << "POST" << " ";
							if (*it4 == PUT)
								std::cout << " " << "PUT" << " ";
							if (*it4 == DELETE)
								std::cout << " " << "DELETE" << " ";
						}
						std::cout << std::endl;
						std::cout << "Error_pages:" << std::endl;
						for (ErrorPages::iterator it4 = it3->error_pages.begin() ; it4 != it3->error_pages.end(); it4++)
						{
							std::cout  << it4->first << "-->";
							std::cout << it4->second << std::endl;
						}
						std::cout << std::endl;
					}
 				}
			}
		}

		Parser::Parser(std::string file_path)
		 : config()
		{
			this->valid = true;
			std::ifstream   fd(file_path);
			std::string     line;
			if (!fd)
			{
				std::cout << "bad file opening." << std::endl;
				this->valid = false;
				return ;
			}
			while (!fd.eof())
			{
				std::getline(fd, line);
				if (line == "server:")
				{
					if (this->mapping_servers(this->config.servers, fd) == 0)
					{
						this->valid = false;
						std::cout << "probleme occured with filling the server info" << std::endl;
					}
				}
			}
			set_default_parameter(this->config.servers);
			if (this->valid == false)
				std::cout << "Parser failure, check args pls." << std::endl;
			else
				print_config(this->config.servers);
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
