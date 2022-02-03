#include "Config.hpp"
#include "Parser.hpp"
#include "return_code.hpp"

namespace ws
{
	namespace conf
	{
        std::string p_route(std::string line){
            if (line[0] == ' ')
                line.erase(1, 9);
            else
                line.erase(0, 9);
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
            line.erase(0,17);
            std::vector<e_method> ret;
            for (size_t i = 0; i <= line.size(); i++)
            {
                if (line[i] == ' ')
                    continue;
                else
                {
                    std::string tmp = line.substr(i, line.size());
                    size_t end = tmp.find_first_of(" ");
                    if (end == std::string::npos)
                        end = tmp.size();
                    std::string tmp2 = tmp.substr(0, end);
                    if (tmp2 == "GET")
                        ret.push_back(GET);
                    else if(tmp2 == "POST")
                        ret.push_back(POST);
                    else if(tmp2 == "PUT")
                        ret.push_back(PUT);
                    else if(tmp2 == "DELETE")
                        ret.push_back(DELETE);
                    i += end;
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

        std::string p_return_code(std::string line){
            line.erase(0, 7);
            size_t b = line.find_first_of("0123456789");
            size_t s = b;
            while (std::isdigit(line[s]))
                s++;
            std::string ret = line.substr(b, s);
            if (ret != TEMPORARY_REDIRECT && ret != SEE_OTHER && ret != MOVED_PERMANENTLY && ret != MOVED_TEMPORARLY)
                return (UNDIFND);
            return (ret);
            
        }

        ErrorPages p_error_pages(ErrorPages errors_pages, std::string line){
            ErrorPages ret(errors_pages);
            line.erase(0, 12);
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
            for (size_t i = 0; i < line.size(); i++)
            {
                if (std::isdigit(line[i]))
                {

                    int tmp = std::stoi(line.substr(i, line.size()));
                    if (tmp > 999 || tmp <= 99)
                    {
                        std::cout << "bad error pages " << std::endl;
                        return (ret);
                    }
                    ret[tmp] = new std::string(path);
                    i +=3;
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