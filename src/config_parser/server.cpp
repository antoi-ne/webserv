#include "../../includes/server.hpp"

server::server(){
}

server::server(const server &src){
    this->_auto_index = src._auto_index;
    this->_client_max_body_size = src._client_max_body_size;
    this->_default = src._default;
    this->_error_pages = src._error_pages;
    this->_ip_addrs = src._ip_addrs;
    this->_location = src._location;
    this->_names = src._names;
    this->_ports = src._ports;
    this->_roots = src._roots;
}

server& server::operator=(const server &src){
    this->_auto_index = src._auto_index;
    this->_client_max_body_size = src._client_max_body_size;
    this->_default = src._default;
    this->_error_pages = src._error_pages;
    this->_ip_addrs = src._ip_addrs;
    this->_location = src._location;
    this->_names = src._names;
    this->_ports = src._ports;
    this->_roots = src._roots;
    return (*this);
}

std::vector<std::string> server::get_names()const{
    return(this->_names);
}

std::vector<std::string> server::get_ip_addrs()const{
    return(this->_ip_addrs);
}

std::vector<u_int16_t> server::get_ports()const{
    return(this->_ports);
}

std::vector<std::string> server::get_roots()const{
    return(this->_roots);
}

std::vector<std::string> server::get_location()const{
    return(this->_location);
}

std::vector<std::string> server::get_error_pages()const{
    return(this->_error_pages);
}

bool server::get_default()const{
    return(this->_default);
}

bool server::get_auto_index()const{
    return(this->_auto_index);
}

int  server::get_client_max_body_size()const{
    return(this->_client_max_body_size);
}

server::~server(){
}
