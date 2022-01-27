#include "Config.hpp"
#include "Parser.hpp"
#include "parser_attr.cpp"

namespace ws
{
	namespace conf
	{
        int count_space(std::string line){
            int i;
            i = 0;
            while (line[i] == ' ')
                i++;
            return (i);
        }

        Location loc_attr(std::fstream fd){
            std::string line;
            Location    ret;
            while (std::getline(fd, line) && line.compare(0, 2, "  "))
            {
                line.erase(0, 2);
                if (line.compare(2, 6, "route:"))
                    //ret.route = p_route(line);
                if (line.compare(2, 5, "root:"))
                    //ret.root = p_root(line);
                if (line.compare(2, 6, "index:"))
                    //ret.index = p_index(line);
                if (line.compare(2, 17, "accepted_methods:"))
                    //ret.accepted_methods = p_accpt_mtde(line);
                if (line.compare(1, 10, "autoindex:"))
                    //ret.autoindex = p_a_index(line);
                if (line.compare(1, 14, "max_body_size:"))
                    //ret.m_bdy_size = p_m_bdy_size(line);
                if (line.compare(1, 12, "error_pages:"))
                    //ret.error_pages = p_error_pages(line);
                if (line.compare(1, 12, "upload_path:"))
                    //ret.upload_path = p_upload_path(line);
                if (line.compare(1, 12, "return_path:"))
                    //ret.return_path = p_return_path(line);
                if (line.compare(1, 12, "return_code:"))
                    //ret.return_code = p_return_code(line);
            }
        }


        int  mapping_servers(std::map<host_port, std::vector<Server> > config, std::ifstream fd){
            std::string line;
            host_port   tmp;

            while (getline(fd, line) && line[0] == ' ')
            {
                line.erase(0, 1);
                if (line.compare(1, 10, "location:\n"))
                    
                if (line.compare(1, 7, "listen:"))
                    map_servers(line, config);
                if (line.compare(1, 12, "server_name:"))
                    //ret.server_names = p_server_names(line);
                if (line.compare(1, 5, "root:"))
                    //ret.root = p_root(line);
                if (line.compare(1, 6, "index:"))
                    //ret.index = p_index(line);
                if (line.compare(1, 10, "autoindex:"))
                    //ret.autoindex = p_a_index();
                if (line.compare(1, 14, "max_body_size:"))
                    //ret.m_bdy_size = p_m_bdy_size();
                if (line.compare(1, 12, "error_pages:"))
                    //p_error_pages(line, ret);
                if (line.compare(1, 12, "upload_path:"))
                    //ret.upload_path = p_upload_path(line);
                if (line.compare(1, 12, "return_path:"))
                    //ret.return_path = p_return_path(line);
                if (line.compare(1, 12, "return_code:"))
                    //ret.return_code = p_return_code(line);
            }
            return (ret);
        }
    }
}