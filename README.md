# static_map
A simple associative array over an enumeration (or any integral type).

# Usage

## Create map over an enumeration
```c++
#include <string>
#include <iostream>
#include "feniks/static_map.hpp"

enum class colors { red, green, blue, count }; // `count` specifies enum size

int main() {
    feniks::static_map<colors, std::string> map;
    
    map.get<colors::red>()   = "red";
    map.get<colors::green>() = "green";
    map.get<colors::blue>()  = "blue";
    
    std::cout << map.get<colors::red>()   << " is " << static_cast<int>(colors::red)   << std::endl;
    std::cout << map.get<colors::green>() << " is " << static_cast<int>(colors::green) << std::endl;
    std::cout << map.get<colors::blue>()  << " is " << static_cast<int>(colors::blue)  << std::endl;
}
```

## Create map over integral type
```c++
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
```

## Map to different types
```c++
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
```