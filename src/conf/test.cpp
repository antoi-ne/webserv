#include <string>
#include <iostream>
std::string p_root(std::string line){
        line.erase(0, 5);
        line.erase(std::remove(line.begin(), line.end(), 'a'), line.end());
        return (line);
}

int main(void){
    std::string test = "root: /ouiiui/lalal/lld/. ";
    std::string rep = p_root(test);
    std::cout << rep << std::endl;
    return (0);
}