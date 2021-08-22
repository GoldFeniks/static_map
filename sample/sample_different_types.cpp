#include <tuple>
#include <string>
#include <iostream>
#include "feniks/static_map.hpp"

enum class colors { red, green, blue, count }; // `count` specifies enum size

int main() {
    feniks::static_map<colors, std::string, feniks::enum_list_t<colors, colors::blue>, std::tuple<double>> map; // blue is mapped to `double` value

    map.get<colors::red>()   = "red";
    map.get<colors::green>() = "green";
    map.get<colors::blue>()  = 42.23; // double

    std::cout << "red is "   << map.get<colors::red>()   << std::endl;
    std::cout << "green is " << map.get<colors::green>() << std::endl;
    std::cout << "blue is "  << map.get<colors::blue>()  << std::endl;
}
