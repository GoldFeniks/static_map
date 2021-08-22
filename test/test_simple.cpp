#include <string>
#include <limits>
#include "gtest/gtest.h"
#include "feniks/static_map.hpp"

enum class simple_enum_count { first, second, third, fourth, count };

TEST(STATIC_MAP_TEST, STATIC_MAP_SIMPLE_COUNT) {
    feniks::static_map<simple_enum_count, std::string> map;

    EXPECT_EQ(map.max_size(), 4);
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());

    map.get<simple_enum_count::first>() = "abcde";
    map.insert<simple_enum_count::second>("edcba");
    map.emplace<simple_enum_count::third>("123");

    EXPECT_TRUE(map.contains<simple_enum_count::first>());
    EXPECT_TRUE(map.contains<simple_enum_count::second>());
    EXPECT_TRUE(map.contains<simple_enum_count::third>());
    EXPECT_FALSE(map.contains<simple_enum_count::fourth>());

    EXPECT_EQ(map.get<simple_enum_count::first>(), "abcde");
    EXPECT_EQ(map.get<simple_enum_count::second>(), "edcba");
    EXPECT_EQ(map.get<simple_enum_count::third>(), "123");

    map.erase<simple_enum_count::second>();
    EXPECT_FALSE(map.contains<simple_enum_count::second>());

    const auto [inserted_value1, did_insert1] = map.insert<simple_enum_count::fourth>("321");
    EXPECT_EQ(inserted_value1, map.get<simple_enum_count::fourth>());
    EXPECT_EQ(map.get<simple_enum_count::fourth>(), "321");
    EXPECT_TRUE(did_insert1);

    const auto [inserted_value2, did_insert2] = map.insert<simple_enum_count::first>("321");
    EXPECT_EQ(inserted_value2, map.get<simple_enum_count::first>());
    EXPECT_EQ(map.get<simple_enum_count::first>(), "abcde");
    EXPECT_FALSE(did_insert2);

    const auto [inserted_value3, did_insert3] = map.emplace<simple_enum_count::third>("321");
    EXPECT_EQ(inserted_value3, map.get<simple_enum_count::third>());
    EXPECT_EQ(map.get<simple_enum_count::third>(), "123");
    EXPECT_FALSE(did_insert3);

    const auto [inserted_value4, did_insert4] = map.insert_or_assign<simple_enum_count::second>("321");
    EXPECT_EQ(inserted_value4, map.get<simple_enum_count::second>());
    EXPECT_EQ(map.get<simple_enum_count::second>(), "321");
    EXPECT_TRUE(did_insert4);

    EXPECT_EQ(map.size(), 4);
    EXPECT_FALSE(map.empty());

    map.clear();
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());
}

enum class simple_enum_numeric_limits { first, second, third, fourth };

template<>
struct std::numeric_limits<simple_enum_numeric_limits> {

    static constexpr simple_enum_numeric_limits min() {
        return simple_enum_numeric_limits::first;
    }

    static constexpr simple_enum_numeric_limits max() {
        return simple_enum_numeric_limits::fourth;
    }

};

TEST(STATIC_MAP_TEST, STATIC_MAP_SIMPLE_NUMERIC_LIMITS) {
    feniks::static_map<simple_enum_numeric_limits, std::string> map;

    EXPECT_EQ(map.max_size(), 4);
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());

    map.get<simple_enum_numeric_limits::first>() = "abcde";
    map.insert<simple_enum_numeric_limits::second>("edcba");
    map.emplace<simple_enum_numeric_limits::third>("123");

    EXPECT_TRUE(map.contains<simple_enum_numeric_limits::first>());
    EXPECT_TRUE(map.contains<simple_enum_numeric_limits::second>());
    EXPECT_TRUE(map.contains<simple_enum_numeric_limits::third>());
    EXPECT_FALSE(map.contains<simple_enum_numeric_limits::fourth>());

    EXPECT_EQ(map.get<simple_enum_numeric_limits::first>(), "abcde");
    EXPECT_EQ(map.get<simple_enum_numeric_limits::second>(), "edcba");
    EXPECT_EQ(map.get<simple_enum_numeric_limits::third>(), "123");

    map.erase<simple_enum_numeric_limits::second>();
    EXPECT_FALSE(map.contains<simple_enum_numeric_limits::second>());

    const auto [inserted_value1, did_insert1] = map.insert<simple_enum_numeric_limits::fourth>("321");
    EXPECT_EQ(inserted_value1, map.get<simple_enum_numeric_limits::fourth>());
    EXPECT_EQ(map.get<simple_enum_numeric_limits::fourth>(), "321");
    EXPECT_TRUE(did_insert1);

    const auto [inserted_value2, did_insert2] = map.insert<simple_enum_numeric_limits::first>("321");
    EXPECT_EQ(inserted_value2, map.get<simple_enum_numeric_limits::first>());
    EXPECT_EQ(map.get<simple_enum_numeric_limits::first>(), "abcde");
    EXPECT_FALSE(did_insert2);

    const auto [inserted_value3, did_insert3] = map.emplace<simple_enum_numeric_limits::third>("321");
    EXPECT_EQ(inserted_value3, map.get<simple_enum_numeric_limits::third>());
    EXPECT_EQ(map.get<simple_enum_numeric_limits::third>(), "123");
    EXPECT_FALSE(did_insert3);

    const auto [inserted_value4, did_insert4] = map.insert_or_assign<simple_enum_numeric_limits::third>("321");
    EXPECT_EQ(inserted_value4, map.get<simple_enum_numeric_limits::third>());
    EXPECT_EQ(map.get<simple_enum_numeric_limits::third>(), "321");
    EXPECT_FALSE(did_insert4);

    EXPECT_EQ(map.size(), 3);
    EXPECT_FALSE(map.empty());

    map.clear();
    EXPECT_EQ(map.size(), 0);
    EXPECT_TRUE(map.empty());
}

TEST(STATIC_MAP_TEST, STATIC_MAP_SIMPLE_CONST) {
    feniks::static_map<simple_enum_count, std::string> map;

    map.get<simple_enum_count::first>() = "first";
    map.get<simple_enum_count::second>() = "second";

    const auto test = [](const feniks::static_map<simple_enum_count, std::string>& map) {
        EXPECT_EQ(map.get<simple_enum_count::first>(), "first");
        EXPECT_EQ(map.get<simple_enum_count::second>(), "second");

        EXPECT_THROW(map.get<simple_enum_count::third>(), std::out_of_range);
        EXPECT_THROW(map.get<simple_enum_count::fourth>(), std::out_of_range);
    };

    test(map);
}

TEST(STATIC_MAP_TEST, STATIC_MAP_SIMPLE_TYPE_OVERRIDE) {
    struct point {
        int x, y, z;

        bool operator==(const point& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    feniks::static_map<simple_enum_count, std::string, feniks::enum_list_t<simple_enum_count, simple_enum_count::second, simple_enum_count::fourth>, std::tuple<int, point>> map;

    map.populate();
    EXPECT_EQ(map.size(), map.max_size());

    EXPECT_TRUE((std::is_same_v<decltype(map.get<simple_enum_count::first>()), std::string&>));
    EXPECT_TRUE((std::is_same_v<decltype(map.get<simple_enum_count::second>()), int&>));
    EXPECT_TRUE((std::is_same_v<decltype(map.get<simple_enum_count::third>()), std::string&>));
    EXPECT_TRUE((std::is_same_v<decltype(map.get<simple_enum_count::fourth>()), point&>));

    map.get<simple_enum_count::first>() = "string";
    map.get<simple_enum_count::second>() = 42;
    map.get<simple_enum_count::third>() = "other string";
    map.get<simple_enum_count::fourth>() = { .x = 23, .y = 42, .z = 4223 };

    EXPECT_EQ(map.get<simple_enum_count::first>(), "string");
    EXPECT_EQ(map.get<simple_enum_count::second>(), 42);
    EXPECT_EQ(map.get<simple_enum_count::third>(), "other string");
    EXPECT_EQ(map.get<simple_enum_count::fourth>(), (point{ 23, 42, 4223 }));
}

TEST(STATIC_MAP_TEST, STATIC_MAP_SIMPLE_ENUM_SPEC) {
    EXPECT_TRUE((std::is_same_v<feniks::static_map<simple_enum_count, int>, feniks::static_map<feniks::enum_specification<simple_enum_count>, int>>));

    feniks::static_map<feniks::enum_specification<simple_enum_count, simple_enum_count::second, simple_enum_count::fourth>, std::string> map_less;
    EXPECT_EQ(map_less.max_size(), 2);
    EXPECT_EQ(sizeof(map_less), sizeof(std::optional<std::string>) * 2);

    enum class enum_non_sequential { first = 1, second = 23, third = 42 };
    using enum_specification = feniks::enum_specification<enum_non_sequential, enum_non_sequential::first, enum_non_sequential::second, enum_non_sequential::third>;
    feniks::static_map<enum_specification, std::string> map;

    EXPECT_EQ(map.max_size(), 3);
    EXPECT_EQ(sizeof(map), sizeof(std::optional<std::string>) * 3);

    map.get<enum_non_sequential::second>() = "first";
    EXPECT_EQ(map.get<enum_non_sequential::second>(), "first");
}

TEST(STATIC_MAP_TEST, STATIC_MAP_SIMPLE_NOT_DEFAULT_CONSTRUCTIBLE) {
    struct point {
        int x, y, z;

        point(const int x, const int y, const int z) : x(x), y(y), z(z) {}

        bool operator==(const point& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    feniks::static_map<simple_enum_count, point> map;

    EXPECT_THROW(map.get<simple_enum_count::first>(), std::out_of_range);

    map.emplace<simple_enum_count::first>(1, 2, 3);
    EXPECT_EQ(map.get<simple_enum_count::first>(), (point(1, 2, 3)));

    map.clear();
    map.populate(3, 2, 1);
    EXPECT_EQ(map.get<simple_enum_count::first>(), (point(3, 2, 1)));
    EXPECT_EQ(map.get<simple_enum_count::second>(), (point(3, 2, 1)));
    EXPECT_EQ(map.get<simple_enum_count::third>(), (point(3, 2, 1)));
    EXPECT_EQ(map.get<simple_enum_count::fourth>(), (point(3, 2, 1)));
}
