#include "Config.hpp"
#include "Parser.hpp"
#include "return_code.hpp"

namespace ws
{
	namespace conf
	{
        std::string p_route(std::string line){
            line.erase(0, 5);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line); 
            
        }

        host_port map_servers(std::string line){
            line.erase(0, 7);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            host_port ret;
            std::size_t b_addr = line.find_first_not_of(" ");
            std::size_t e_addr = line.find_first_of(":");
            if (e_addr == std::string::npos)
            {
                ret.first = "0.0.0.0";
                ret.second = std::stoi(line);
            }
            else
            {
                ret.first = line.substr(b_addr, e_addr);
                ret.second = std::stoi(line.substr((e_addr + 1), line.size()));
            }
            return (ret);
            
        }

        std::string p_root(std::string line){
            line.erase(0, 5);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::string p_index(std::string line){
            line.erase(0, 6);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::vector<e_method> p_accpt_mtde(std::string line){
            std::vector<e_method> ret;
            for (size_t i = 0; i < line.size(); i++)
            {
                if (line[i] == ' ')
                    continue;
                else
                {
                    std::string tmp = line.substr(i, line.find_first_of(" "));
                    if (tmp == "GET")
                        ret.push_back(GET);
                    else if(tmp == "POST")
                        ret.push_back(POST);
                    else if(tmp == "PUT")
                        ret.push_back(PUT);
                    else if(tmp == "DELETE")
                        ret.push_back(DELETE);
                    i += line.find_first_of(" ");
                }
            }
            return (ret);
        }

        bool p_a_index(std::string line){
            line.erase(0, 10);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            if (line == "on")
                return (true);
            return (false);
        }

        int p_m_bdy_size(std::string line){
            line.erase(0, 21);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            int ret = std::stoi(line);
            return (ret);
        }

        std::string p_upload_path(std::string line){
            line.erase(0, 12);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::string p_return_path(std::string line){
            line.erase(0, 12);
            size_t begin = line.find_first_of("abcdefghijklmnopqrstuvwxyz/");
            if (begin == std::string::npos)
                return ("");
            std::string tmp = line.substr(begin, line.size());
            size_t end = line.find_first_of(" ");
            if (end == std::string::npos)
                end = line.size();
            tmp = tmp.substr(begin, end);
            return (tmp);
        }

        return_code p_return_code(std::string line){
            line.erase(0, 7);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            int ret = std::stoi(line);
            if (ret == MOVED_PERMANENTLY)
                return MOVED_PERMANENTLY;
            if (ret == MOVED_TEMPORARLY)
                return MOVED_TEMPORARLY;
            if (ret == SEE_OTHER)
                return SEE_OTHER;
            if (ret == TEMPORARY_REDIRECT)
                return TEMPORARY_REDIRECT;
            return (UNDIFND);
        }

        ErrorPages p_error_pages(ErrorPages errors_pages, std::string line){
            ErrorPages ret(errors_pages);
            size_t s = line.size();
            while (line[s] == ' ')
                s--;
            if (s != line.size())
                line.erase(s, line.size());
            if ((s = line.find_last_of(" ")) == std::string::npos)
            {
                if ((s = line.find_first_of("abcdefghijklmnopqrstuvwxyz/")) == std::string::npos)
                {
                    std::cout << "Error page, bad syntaxe" << std::endl;
                    return (ret);
                }
            }
            std::string path = line.substr(s, line.size());
            line.erase(s, line.size());
            std::cout << path << std::endl;
            for (size_t i = 0; i < line.size(); i++)
            {
                if (std::isdigit(line[i]))
                {
                    size_t end = line.find_first_of(" ") - 1;
                    if (end == (std::string::npos - 1))
                    {
                        std::cout << "Error page, bad syntaxe" << std::endl;
                        return (ret);                   
                    } 
                    std::cout << line.substr(i, end) << std::endl;
                    int tmp = std::stoi(line.substr(i, end));
                    std::cout << tmp << std::endl;
                    ret[tmp] = new std::string(path);
                    i += end;
                }
            }
            return (ret);
        }

        std::vector<std::string> p_server_names(std::string line){
            line.erase(0, 13);
            std::vector <std::string> ret;
            for (size_t i = 0; i < line.size(); i++)
            {
                if (line[i] == ' ')
                    continue;
                else
                {
                    std::string strtmp = line.substr(i, line.size());
                    size_t end = strtmp.find_first_not_of("_abcdefghiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.");
                    if (end == std::string::npos)
                        end = strtmp.size();
                    std::string tmp = line.substr(i, end);
                    ret.push_back(tmp);
                    i += tmp.size();
                }
            }
            return (ret);
        }
    }
}