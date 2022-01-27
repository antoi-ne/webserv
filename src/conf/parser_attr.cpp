#include "config.hpp"
#include "Parser.hpp"

namespace ws
{
	namespace conf
	{
        std::string p_route(std::string line){
            line.erase(0, 5);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line); 
            
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
        }

        bool p_a_index(std::string line){
            line.erase(0, 10);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            if (line == "on")
                return (true);
            return (false);
        }

        int p_m_bdy_size(std::string line){
            line.erase(0, 20);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            int ret = std::stoi(line);
            return (ret);
        }

        std::map<unsigned int,std::string> p_error_pages(std::string line){

        }

        std::string p_upload_path(std::string line){
            line.erase(0, 12);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        std::string p_return_path(std::string line){
            line.erase(0, 12);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            return (line);
        }

        int p_return_code(std::string line){
            line.erase(0, 7);
            line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
            int ret = std::stoi(line);
            return (ret);
        }

        std::vector<std::string> p_server_names(std::string line){

        }

        std::string p_host(std::string line, Server ret){
            line.erase(0, 7);
            std::size_t i = line.find_first_not_of(" ");


        }

    }
}