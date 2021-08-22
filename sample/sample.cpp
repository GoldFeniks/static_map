#include <string>
#include <iostream>
#include "feniks/static_map.hpp"

enum class sample_enum { first, second, third, count }; // `count` specifies enum size

int main() {
    feniks::static_map<sample_enum, std::string> map;

    map.get<sample_enum::first>() = "second";
    map.get<sample_enum::second>() = "first";

    std::cout << "First value: " << map.get<sample_enum::first>() << std::endl;
    std::cout << "Second value: " << map.get<sample_enum::second>() << std::endl;
    std::cout << "Has third value: " << std::boolalpha << map.contains<sample_enum::third>() << std::endl;
}
