#include <vector>
#include <string>


class server{
    public:
        server();
        server(const server &src);
        server& operator=(const server &src);
        std::vector<std::string> get_names()const;
        std::vector<std::string> get_ip_addrs()const;
        std::vector<u_int16_t> get_ports()const;
        std::vector<std::string> get_roots()const;
        std::vector<std::string> get_location()const;
        std::vector<std::string> get_error_pages()const;
        bool get_default()const;
        bool get_auto_index()const;
        int  get_client_max_body_size()const;
        ~server();
    private:
        std::vector<std::string>    _names;
        std::vector<std::string>    _ip_addrs;
        std::vector<u_int16_t>    _ports;
        std::vector<std::string>    _roots;
        std::vector<std::string>    _location;
        std::vector<std::string>    _error_pages;
        bool                        _default;
        bool                        _auto_index;
        int                         _client_max_body_size;
};