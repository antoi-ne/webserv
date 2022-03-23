
#include "Parser.hpp"
#include "Config.hpp"

# define FIRST_OFF 1
# define FIRST_NOT_OFF 2
# define LAST_OFF 3
# define LAST_NOT_OFF 4

size_t finder(std::string line, std::string tofind, int wich){
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

int main(void)
{

    //std::cout << finder(std::string("aaaaaabbbbbbccccccccddddddeeeeeeee"), std::string("a"), 2) << std::endl;
    ws::conf::Parser parser;
    return (0);
}
