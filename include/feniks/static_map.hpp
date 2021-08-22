#pragma once
#include <tuple>
#include <limits>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <type_traits>

namespace feniks {


    template<typename Enum, Enum... Values>
    struct enum_specification;


    namespace implementation {

        template<typename Enum>
        class has_enum_count {

        private:

            template<typename C>
            static char test(std::add_pointer_t<decltype(std::declval<C>() == C::count)>) { return 0; }

            template<typename C>
            static int  test(...) { return 0; }

        public:

            static constexpr bool value = std::is_same_v<decltype(test<Enum>(nullptr)), char>;

        };

        template<typename Enum>
        inline static constexpr bool has_enum_count_v = has_enum_count<Enum>::value;


        template<typename, bool>
        struct enum_count;

        template<typename Enum>
        struct enum_count<Enum, true> {

            inline static constexpr Enum value = Enum::count;

        };

        template<typename Enum>
        struct enum_count<Enum, false> {

            inline static constexpr Enum value = Enum(static_cast<std::underlying_type_t<Enum>>(std::numeric_limits<Enum>::max()) + 1);

        };

        template<typename Enum>
        inline static constexpr Enum enum_count_v = enum_count<Enum, has_enum_count_v<Enum>>::value;


        template<typename, typename>
        struct tuple_prepend;

        template<typename T, typename... Values>
        struct tuple_prepend<T, std::tuple<Values...>> {

            using type = std::tuple<T, Values...>;

        };

        template<typename T, typename Tuple>
        using tuple_prepend_t = typename tuple_prepend<T, Tuple>::type;


        template<typename Enum, typename Value>
        struct enum_enumeration;

        template<typename Enum>
        struct enum_enumeration<Enum, std::integral_constant<Enum, enum_count_v<Enum>>> {

            using type = std::tuple<>;

        };

        template<typename Enum, Enum Value>
        struct enum_enumeration<Enum, std::integral_constant<Enum, Value>> {

            using next = std::integral_constant<Enum, Enum(static_cast<std::underlying_type_t<Enum>>(Value) + 1)>;
            using type = tuple_prepend_t<std::integral_constant<Enum, Value>, typename enum_enumeration<Enum, next>::type>;

        };

        template<typename Enum, Enum Value = std::numeric_limits<Enum>::min()>
        using enum_enumeration_t = typename enum_enumeration<Enum, std::integral_constant<Enum, Value>>::type;


        template<typename Enum, Enum, typename, typename...>
        struct enum_value_type;

        template<typename Enum, Enum Key, typename Value, Enum KeyOverride, typename ValueOverride, typename... Overrides>
        struct enum_value_type<Enum, Key, Value, std::tuple<std::integral_constant<Enum, KeyOverride>, ValueOverride>, Overrides...> {

            using type = std::enable_if_t<Key != KeyOverride, typename enum_value_type<Enum, Key, Value, Overrides...>::type>;

        };

        template<typename Enum, Enum Key, typename Value, typename ValueOverride, typename... Overrides>
        struct enum_value_type<Enum, Key, Value, std::tuple<std::integral_constant<Enum, Key>, ValueOverride>, Overrides...> {

            using type = ValueOverride;

        };

        template<typename Enum, Enum Key, typename Value>
        struct enum_value_type<Enum, Key, Value> {

            using type = Value;

        };

        template<typename Enum, Enum Key, typename Value, typename... Overrides>
        using enum_value_type_t = typename enum_value_type<Enum, Key, Value, Overrides...>::type;


        template<typename Enum, Enum, typename Value>
        struct enum_value {

            constexpr enum_value() = default;

            std::optional<Value> value;

        };

        template<typename, typename, typename, typename, typename>
        struct static_map;

        template<typename Enum, typename Value, Enum... Keys, typename... KeyOverrides, typename... TypeOverrides>
        struct static_map<std::enable_if_t<sizeof...(KeyOverrides) == sizeof...(TypeOverrides), Enum>, Value, std::tuple<std::integral_constant<Enum, Keys>...>, std::tuple<KeyOverrides...>, std::tuple<TypeOverrides...>> : private enum_value<Enum, Keys, enum_value_type_t<Enum, Keys, Value, std::tuple<KeyOverrides, TypeOverrides>...>>... {

            using size_type = size_t;

            template<Enum Key>
            using value_type = enum_value_type_t<Enum, Key, Value, std::tuple<KeyOverrides, TypeOverrides>...>;

            template<Enum Key>
            using enum_value = implementation::enum_value<Enum, Key, value_type<Key>>;

            constexpr static_map() = default;

            template<Enum Key>
            [[nodiscard]] constexpr bool contains() const {
                return enum_value<Key>::value.has_value();
            }

            template<Enum Key>
            constexpr auto& get() {
                auto& optional = enum_value<Key>::value;

                if (!optional.has_value()) {
                    if constexpr (std::is_default_constructible_v<Value>)
                        optional = value_type<Key>();
                    else
                        throw std::out_of_range("There's no value for the specified key"
                                                "and underline type is not default constructible."
                                                "Use insert or emplace to add a value.");
                }

                return optional.value();
            }

            template<Enum Key>
            constexpr const auto& get() const {
                auto& optional = enum_value<Key>::value;

                if (!optional.has_value())
                    throw std::out_of_range("There's no value for the specified key");

                return optional.value();
            }

            [[nodiscard]] constexpr size_type size() const noexcept {
                return (0 + ... + enum_value<Keys>::value.has_value());
            }

            [[nodiscard]] constexpr bool empty() const noexcept {
                return (!enum_value<Keys>::value.has_value() && ...);
            }

            [[nodiscard]] constexpr size_type max_size() const noexcept {
                return sizeof...(Keys);
            }

            constexpr void clear() noexcept {
                (enum_value<Keys>::value.reset(), ...);
            }

            template<Enum Key, typename P>
            constexpr std::pair<value_type<Key>&, bool> insert(P&& value) {
                auto& optional = enum_value<Key>::value;
                bool result = false;

                if (!optional.has_value()) {
                    optional = std::forward<P>(value);
                    result = true;
                }

                return { optional.value(), result };
            }

            template<Enum Key, typename M>
            constexpr std::pair<value_type<Key>&, bool> insert_or_assign(M&& value) {
                auto& optional = enum_value<Key>::value;
                const bool result = !optional.has_value();

                optional = std::forward<M>(value);

                return { optional.value(), result };
            }

            template<Enum Key, typename... Args>
            constexpr std::pair<value_type<Key>&, bool> emplace(Args&&... args) {
                auto& optional = enum_value<Key>::value;
                bool result = false;

                if (!optional.has_value()) {
                    optional.template emplace(std::forward<Args>(args)...);
                    result = true;
                }

                return { optional.value(), result };
            }

            template<Enum Key>
            constexpr void erase() {
                enum_value<Key>::value.reset();
            }

            template<typename... Args>
            constexpr void populate(const Args&... args) {
                ((enum_value<Keys>::value = value_type<Keys>(args...)), ...);
            }

        };


        template<typename Enum, typename Value, typename KeyOverrides, typename TypeOverrides>
        struct static_map_type {

            using specification = enum_specification<Enum>;
            using type = static_map<typename specification::type, Value, typename specification::values, KeyOverrides, TypeOverrides>;

        };

        template<typename Enum, Enum... Values, typename Value, typename KeyOverrides, typename TypeOverrides>
        struct static_map_type<enum_specification<Enum, Values...>, Value, KeyOverrides, TypeOverrides> {

            using type = static_map<Enum, Value, typename enum_specification<Enum, Values...>::values, KeyOverrides, TypeOverrides>;

        };

    }// namespace implementation


    template<typename Enum, Enum... Values>
    struct enum_specification {

        using type = Enum;

        using values = std::tuple<std::integral_constant<Enum, Values>...>;

    };

    template<typename Enum>
    struct enum_specification<Enum> {

        using type = Enum;

        using values = implementation::enum_enumeration_t<Enum>;

    };


    template<typename Enum, Enum... Values>
    struct enum_list {

        using type = std::tuple<std::integral_constant<Enum, Values>...>;

    };

    template<typename Enum, Enum... Values>
    using enum_list_t = typename enum_list<Enum, Values...>::type;


    template<typename Enum, typename Value, typename KeyOverrides = std::tuple<>, typename TypeOverrides = std::tuple<>>
    using static_map = typename implementation::static_map_type<Enum, Value, KeyOverrides, TypeOverrides>::type;

}// namespace feniks
