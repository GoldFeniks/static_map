#include <string>
#include <iostream>
#include "feniks/static_map.hpp"

int main() {
    feniks::static_map<feniks::enum_specification<int, 23, 42>, std::string> map; // all possible keys are specified explicitly

    map.get<23>() = "42";
    map.get<42>() = "23";

    std::cout << map.get<23>() << std::endl;
    std::cout << map.get<42>() << std::endl;
}
