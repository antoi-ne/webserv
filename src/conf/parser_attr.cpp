#include "Config.hpp"
#include "Parser.hpp"
#include "return_code.hpp"

# define FIRST_OFF 1
# define FIRST_NOT_OFF 2
# define LAST_OFF 3
# define LAST_NOT_OFF 4


namespace ws
{
	namespace conf
	{
        size_t finder(std::string line, const char *tofind, int wich){
            size_t ret = -1;
            for (size_t i = 0; line[i]; i++)
            {
                if (wich == FIRST_OFF)
                {
                    for (size_t j = 0; tofind[j]; j++)
                    {
                        if (line[i] == tofind[j])
                            return (i);
                    }
                }
                if (wich == FIRST_NOT_OFF)
                {
                    for (size_t j = 0; tofind[j]; j++)
                    {
                        if (line[i] != tofind[j])
                            return (i);
                    }
                }
                if (wich == LAST_OFF)
                {
                    for (size_t j = 0; tofind[j]; j++)
                    {
                        if (line[i] == tofind[j])
                            ret = i;
                    }
                }
                if (wich == LAST_NOT_OFF)
                {
                    for (size_t j = 0; tofind[j]; j++)
                    {
                        if (line[i] != tofind[j])
                            ret = i;
                    }
                }
            }
            return (ret);
        }
        size_t find_ascii(std::string line, int wich){
            size_t ret = 1;
            for(int i; line[i]; i++)
            {
                if (wich == FIRST_OFF && line[i] >= 32 && line[i] < 127)
                    return (i);
                else if (wich == LAST_OFF)
                {
                    if (line[i] >= 32 && line[i] < 127)
                        ret = i;
                }
                i++;
            }
            return (ret);
        }


        std::string Parser::p_route(std::string line){
            if (line[0] == ' ')
                line.erase(1, 9);
            else
                line.erase(0, 9);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line); 
            
        }

        host_port Parser::map_servers(std::string line){

            line.erase(0, 7);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            host_port ret;
            size_t b_addr = finder(line, " ", FIRST_NOT_OFF);
            size_t e_addr = finder(line, ":", FIRST_OFF);
            if (e_addr == -1)
            {
                ret.first = "0.0.0.0";
                try{ret.second = std::stoi(line);}
                catch(...){
                    this->valid = false;
                    std::cout << "bad port number" <<std::endl;
                }
            }
            else
            {
                try{ret.first = line.substr(b_addr, e_addr);
                ret.second = std::stoi(line.substr((e_addr + 1), line.size()));
                }
                catch(...){
                    this->valid = false;
                    std::cout << "bad port number" << std::endl;}
            }
            return (ret);
            
        }

        std::string Parser::p_root(std::string line){
            line.erase(0, 5);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::string Parser::p_index(std::string line){
            line.erase(0, 6);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::string Parser::p_cgi_pass(std::string line){
            line.erase(0,9);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::string Parser::p_cgi_ext(std::string line){
            line.erase(0,8);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::vector<e_method> Parser::p_accpt_mtde(std::string line){
            try{
            line.erase(0,17);
            std::vector<e_method> ret;
            for (size_t i = 0; i <= line.size(); i++)
            {
                if (line[i] == ' ')
                    continue;
                else
                {
                    std::string tmp = line.substr(i, line.size());
                    size_t end = finder(tmp, " ", FIRST_OFF);
                    if (end == -1)
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
            catch(...){
                std::vector<e_method> ret;
                std::cout << "accepted method error" << std::endl;
                this->valid = false;
                return (ret);
            }
        }

        bool Parser::p_a_index(std::string line){
            line.erase(0, 10);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            if (line == "on")
                return (true);
            return (false);
        }

        int Parser::p_m_bdy_size(std::string line){
            line.erase(0, 21);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            try
                {return (std::stoi(line));}
            catch(...)
            {
                this->valid = false;
                std::cout << "max body size problem: number's conversion is not possible" << std::endl;
                return (0);
            }
        }

        std::string Parser::p_upload_path(std::string line){
            line.erase(0, 12);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::string Parser::p_return_path(std::string line){
            try{
                line.erase(0, 12);
            size_t begin = finder(line, "abcdefghijklmnopqrstuvwxyz/", FIRST_OFF);
            if (begin == -1)
            {
                std::cout << "return path problem" << std::endl;
                this->valid = false;
                return ("");
            }
            std::string tmp = line.substr(begin, line.size());
            size_t end = finder(line, " ", FIRST_OFF);
            if (end == -1)
                end = line.size();
            tmp = tmp.substr(begin, end);
            return (tmp);
            }
            catch(...){
                std::cout << "return path substr problem" << std::endl;
                this->valid = false;
                return(std::string(""));
            }
        }

        std::string Parser::p_return_code(std::string line){
            try{
            line.erase(0, 7);
            size_t b = finder(line, "0123456789", FIRST_OFF);
            if (b == -1)
                throw("bad");
            size_t s = b;
            while (std::isdigit(line[s]))
                s++;
            s--;
            std::string ret = line.substr(b, s);
            if (ret != TEMPORARY_REDIRECT && ret != SEE_OTHER && ret != MOVED_PERMANENTLY && ret != MOVED_TEMPORARLY)
                return (UNDIFND);
            return (ret);
            }
            catch(...){
                std::cout << "return's code wrong parameter" << std::endl;
                this->valid = false;
                return(std::string(""));
            }
            
        }

        ErrorPages Parser::p_error_pages(ErrorPages errors_pages, std::string line){
            try{
                ErrorPages ret(errors_pages);
                line.erase(0, 12);
                size_t s = line.size();
                while (line[s] == ' ')
                    s--;
                if (s != line.size())
                    line.erase(s, line.size());
                if ((s = line.find_last_of(" ")) == std::string::npos)
                {
                    if ((s = find_ascii(line, FIRST_OFF) == line.size()))
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
                        std::string tmp = line.substr(i, 3);
                        size_t lastof = path.find_last_of("/");
                        path.insert(lastof + 1, tmp);
                    
                        int tmpint = std::stoi(tmp);
                        ret[tmpint] = path;
                        path.erase(lastof + 1, 3);
                        i += 3;    
                    }
                }
                return (ret);
            }
            catch(...){
                this->valid = false;
                std::cout << "bad error page num" << std::endl;
                return (ErrorPages());
                }
        }

        std::vector<std::string> Parser::p_server_names(std::string line){
           try{
                line.erase(0, 13);
                std::vector <std::string> ret;
                for (size_t i = 0; i < line.size(); i++)
                {
                    if (line[i] == ' ')
                        continue;
                    else
                    {
                        std::string strtmp = line.substr(i, line.size());
                        size_t end = strtmp.find_first_not_of("_abcdefghiklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ.0123456789");
                        if (end == std::string::npos)
                            end = strtmp.size();
                        std::string tmp = line.substr(i, end);
                        ret.push_back(tmp);
                        i += tmp.size();
                    }
                }
                return (ret);
           }
           catch(...){
               std::vector<std::string> ret;
               this->valid = false;
               std::cout << "server_names error" << std::endl;
               return(ret);
           }
        }
    }
}