#include "../../includes/config.hpp"

config::config(){
}

config::config(const config &src){
    this->server_lst = src.server_lst;
}

config& config::config(const config &src){
    this->server_lst = src.server_lst;
    return (*this);
}

config::~config(){
}