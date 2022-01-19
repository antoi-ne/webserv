#include "server.hpp"

struct config{
    public:
        config();
        config(const config &src);
        config& operator=(const config &src);
        ~config();
    private:
        std::vector<server> server_lst;
};