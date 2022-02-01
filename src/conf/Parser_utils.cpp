#include "Config.hpp"
#include "Parser.hpp"


namespace ws
{
	namespace conf
	{
        Location loc_attr(std::ifstream &fd){
            std::string line;
            Location    ret;
            while (std::getline(fd, line) && (line[0] == '\0' || !(line.compare(0, 2, "  "))))
            {
                if (line[0] == '\0')
                    continue;
                line.erase(0, 2);
                std::cout << "test" << line << std::endl;
                if (line.compare(2, 6, "route:"))
                    //ret.route = p_route(line);
                if (line.compare(2, 5, "root:"))
                    ret.root = p_root(line);
                if (line.compare(2, 6, "index:"))
                    ret.index = p_index(line);
                if (line.compare(2, 17, "accepted_methods:"))
                {
                    ret.accepted_methods.push_back(e_method(2));
                    //ret.accepted_methods = p_accpt_mtde(line);
                }
                if (line.compare(1, 10, "autoindex:"))
                    ret.autoindex = p_a_index(line);
                if (line.compare(1, 14, "max_body_size:"))
                    ret.max_body_size = p_m_bdy_size(line);
                if (line.compare(1, 12, "error_pages:"))
                    //ret.error_pages = p_error_pages(line);
                if (line.compare(1, 12, "upload_path:"))
                    ret.upload_path = p_upload_path(line);
                if (line.compare(1, 12, "return_path:"))
                    ret.return_path = p_return_path(line);
                if (line.compare(1, 12, "return_code:"))
                    ret.return_code = p_return_code(line);
            }
            return (ret);
        }


        void  mapping_servers(server_map config, std::ifstream &fd){
            std::string line;
            host_port   tmp;
            Server      tmp_server;

            while (getline(fd, line) && (line[0] == ' ' || line[0] == '\0'))
            {
                if (line[0] == '\0')
                    continue;
                line.erase(0, 1);
                if (line.compare(1, 7, "listen:"))
                    tmp = map_servers(line);
                if (line.compare(1, 10, "location:\n"))
                    tmp_server.locations.push_back(loc_attr(fd));
                if (line.compare(1, 12, "server_name:"))
                   // tmp_server.server_names = p_server_names(line);
                if (line.compare(1, 5, "root:"))
                    tmp_server.root = p_root(line);
                if (line.compare(1, 6, "index:"))
                    tmp_server.index = p_index(line);
                if (line.compare(1, 10, "autoindex:"))
                    tmp_server.autoindex = p_a_index(line);
                if (line.compare(1, 14, "max_body_size:"))
                    tmp_server.max_body_size = p_m_bdy_size(line);
                if (line.compare(1, 12, "error_pages:"))
                    //p_error_pages(line, ret);
                if (line.compare(1, 12, "upload_path:"))
                    tmp_server.upload_path = p_upload_path(line);
                if (line.compare(1, 12, "return_path:"))
                    tmp_server.return_path = p_return_path(line);
                if (line.compare(1, 12, "return_code:"))
                    tmp_server.return_code = p_return_code(line);
            }
            config[tmp].push_back(tmp_server);
        }
    }
}