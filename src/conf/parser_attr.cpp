#include "config.hpp"
#include "Parser.hpp"

namespace ws
{
	namespace conf
	{
        std::string p_route(std::string line){
            line.erase(0, 5);
            line.erase(std::remove(line.begin(), line.end(), 'a'), line.end());
            return (line); 
            
        }

        std::string p_root(std::string line){
            line.erase(0, 5);
            line.erase(std::remove(line.begin(), line.end(), 'a'), line.end());
            return (line);
        }

        std::string p_index(std::string line){
            line.erase(0, 6);
            line.erase(std::remove(line.begin(), line.end(), 'a'), line.end());
            return (line);
        }

        std::vector<Methods> p_accpt_mtde(std::string line){

        }

        bool p_a_index(std::string line){

        }

        int p_m_bdy_size(std::string line){

        }

        std::map<unsigned int,std::string> p_error_pages(std::string line){

        }

        std::string p_upload_path(std::string line){
            line.erase(0, 12);
            line.erase(std::remove(line.begin(), line.end(), 'a'), line.end());
            return (line);
        }

        std::string p_return_path(std::string line){
            line.erase(0, 12);
            line.erase(std::remove(line.begin(), line.end(), 'a'), line.end());
            return (line);
        }

        int p_return_code(std::string line){

        }

        std::vector<std::string> p_server_names(std::string line){

        }

        std::string p_host(std::string ling, Server ret){

        }

    }
}