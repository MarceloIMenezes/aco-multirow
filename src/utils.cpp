#include "utils.hpp"

std::vector<std::string> split(std::string str, char del) {
    std::vector<std::string> s;
    std::stringstream ss(str);
    std::string part;
    while(getline(ss, part, del)) {
        s.push_back(part);
    }
    return s;
}