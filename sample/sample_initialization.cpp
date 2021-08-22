#include <string>
#include <iostream>
#include "feniks/static_map.hpp"

enum class colors { red, green, blue, count }; // `count` specifies enum size

int main() {
    feniks::static_map<colors, std::string> map{ { colors::red, "red" }, { colors::green, "green" }, { colors::blue, "blue" } };

    std::cout << map.get<colors::red>()   << " is " << static_cast<int>(colors::red)   << std::endl;
    std::cout << map.get<colors::green>() << " is " << static_cast<int>(colors::green) << std::endl;
    std::cout << map.get<colors::blue>()  << " is " << static_cast<int>(colors::blue)  << std::endl;
}
