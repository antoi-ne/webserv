#include "Config.hpp"
#include "Parser.hpp"


namespace ws
{
	namespace conf
	{
        std::string Parser::loc_attr(std::ifstream &fd, Server &server, std::string prev_line)
        {
            std::string line;
            Location    ret;
            ret.route = p_route(prev_line);
            ret.autoindex = false;
            ret.max_body_size = -1;
            while (std::getline(fd, line) && ( line[0] == '\0' ||!(line.compare(0, 2, "  "))))
            {
                if (line[0] == '\0')
                    continue;
                line.erase(0, 2);
                if (!(line.compare(0, 5, "root:"))) 
                    ret.root = p_root(line);
                if (!(line.compare(0, 6, "index:")))
                    ret.index = p_index(line);
                if (!(line.compare(0, 17, "accepted_methods:")))
                    ret.accepted_methods = p_accpt_mtde(line);
                if (!(line.compare(0, 10, "autoindex:")))
                    ret.autoindex = p_a_index(line);
                if (!(line.compare(0, 21, "client_max_body_size:")))
                    ret.max_body_size = p_m_bdy_size(line);
                if (!(line.compare(0, 12, "error_pages:")))
                    ret.error_pages = p_error_pages(ret.error_pages, line);
                if (!(line.compare(0, 12, "upload_path:")))
                    ret.upload_path = p_upload_path(line);
                if (!(line.compare(0, 7, "return:")))
                    ret.return_path = p_return_path(line);
                if (!(line.compare(0, 9, "cgi_pass:")))
                    ret.cgi_script = p_cgi_pass(line);
                if (!(line.compare(0, 8, "cgi_ext:")))
                    ret.cgi_ext = p_cgi_ext(line);
                if (!(line.compare(0, 7, "return:")))
                    ret.return_code = p_return_code(line);
            }
            if ((ret.cgi_ext.empty() && !(ret.cgi_script.empty())) || (!(ret.cgi_ext.empty()) && ret.cgi_script.empty()))
            {
                this->valid = false;
                throw ("cgi error");
            }
            server.locations.push_back(ret);
            if (!(line.compare(1, 9, "location:")))
                return loc_attr(fd, server, line);
            return (line);
        }

        int  Parser::mapping_servers(server_map &config, std::ifstream &fd){
            std::string line;
            host_port   tmp;
            Server      tmp_server;
            
            tmp_server.autoindex = false;
            tmp_server.max_body_size = -1;
            while (getline(fd, line) && (line[0] == '\0' || line[0] == ' '))
            {
                if (line[0] == '\0')
                    continue;
                line.erase(0, 1);
                if (!(line.compare(0, 7, "listen:"))) {
                    tmp = map_servers(line);
                }
                if (!(line.compare(0, 9, "location:")))
                {
                    line = loc_attr(fd, tmp_server, line); 
                    if (!(line.compare(0, 7,"server:")) || (line[0] != '\0' && line[0] != ' '))
                        break;
                    else
                        line.erase(0, 1);
                    std::cout << line << std::endl;
                }
                if (!(line.compare(0, 13, "server_names:")))
                    tmp_server.server_names = p_server_names(line);
                if (!(line.compare(0, 5, "root:")))
                    tmp_server.root = p_root(line);
                if (!(line.compare(0, 6, "index:")))
                    tmp_server.index = p_index(line);
                if (!(line.compare(0, 10, "autoindex:")))
                    tmp_server.autoindex = p_a_index(line);
                if (!(line.compare(0, 21, "client_max_body_size:")))
                    tmp_server.max_body_size = p_m_bdy_size(line);
                if (!(line.compare(0, 12, "error_pages:")))
                    tmp_server.error_pages = p_error_pages(tmp_server.error_pages, line);
                if (!(line.compare(0, 12, "upload_path:")))
                    tmp_server.upload_path = p_upload_path(line);
                if (!(line.compare(0, 7, "return:")))
                {
                    tmp_server.return_path = p_return_path(line);
                    tmp_server.return_code = p_return_code(line);
                }
            } 
            config[tmp].push_back(tmp_server);
           if (line == "server:")
                return (mapping_servers(config, fd));
            return (1);
        }
    }
}