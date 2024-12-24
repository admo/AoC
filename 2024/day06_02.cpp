#include <algorithm>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>

using std::istream_iterator;
using std::ranges::find_first_of;
using std::views::drop;
using std::views::reverse;
using std::views::stride;
using std::views::take;
using std::string_view_literals::operator""sv;

inline constexpr auto dir_char = "^>v<"sv;
inline constexpr auto dir_mod = std::array{std::array{0, -1}, std::array{1, 0}, std::array{0, 1}, std::array{-1, 0}};

auto read_map_from(const std::string &filename)
{
    auto file = std::ifstream{filename};
    std::string map;

    if (!std::getline(file, map))
        throw std::runtime_error{"empty file"};

    const auto width = map.size();
    map.reserve(128 * width);

    for (std::string line; std::getline(file, line);)
        map.append(line);

    const auto p = map.find_first_of(dir_char);
    const auto [x, y] = std::pair{p % width, p / width};
    return std::make_tuple(std::move(map), width, x, y, dir_char.find(map[p]), true);
}

template <typename V>
auto go(V view, const std::size_t x, const std::size_t y, const std::size_t d)
{
    // Find the first obstacle in the path
    const auto p = std::ranges::find_first_of(view, "#O"sv);

    // If the guard already visited this place, she is in a loop
    const auto distance = std::distance(view.begin(), p) - 1;
    const auto in_loop = distance > 0 && *(p - 1) == 'X';

    // Fill the path with X as visited by the guard
    std::ranges::fill(view.begin(), p, 'X');
    return std::make_tuple(x + dir_mod[d][0] * distance, y + dir_mod[d][1] * distance, p == view.end(), in_loop);
}

std::size_t solve(std::string m, const std::size_t w, std::size_t x, std::size_t y, std::size_t direction,
           const bool try_obstacles = false)
{
    using GoToFun = std::function<std::tuple<std::size_t, std::size_t, bool, bool>(std::size_t, std::size_t, bool)>;

    const auto goto_functions = std::array{
        GoToFun{[&m, w](const std::size_t _x, const std::size_t _y, const bool try_obstacles) // Nort
                {
                    const auto v = m | take(_y * w + _x + 1) | drop(_x) | stride(w) | reverse;
                    if (try_obstacles)
                        for (auto &f : v | drop(1) | std::views::take_while([](auto c) { return c != '#'; }))
                        {
                            const auto previous = std::exchange(f, 'O');
                            if(0 == solve(m, w, _x, _y, 0))
                                std::cout << "in loop " << _x << ' ' << _y - std::ranges::distance(v.begin(), std::ranges::find(v, 'O')) << '\n';
                            f = previous;
                        }
                    return go(v, _x, _y, 0);
                }},
        GoToFun{[&m, w](const std::size_t _x, const std::size_t _y, const bool try_obstacles) // East
                {
                    const auto v = m | drop(_y * w + _x) | take(w - _x);
                    if (try_obstacles)
                        for (auto &f : v | drop(1) | std::views::take_while([](auto c) { return c != '#'; }))
                        {
                            const auto previous = std::exchange(f, 'O');
                            if(0 == solve(m, w, _x, _y, 1))
                                std::cout << "in loop " << _x + std::ranges::distance(v.begin(), std::ranges::find(v, 'O')) << ' ' << _y << '\n';
                            f = previous;
                        }
                    return go(v, _x, _y, 1);
                }},
        GoToFun{[&m, w](const std::size_t _x, const std::size_t _y, const bool try_obstacles) // South
                {
                    const auto v = m | drop(_y * w + _x) | stride(w);
                    if (try_obstacles)
                        for (auto &f : v | drop(1) | std::views::take_while([](auto c) { return c != '#'; }))
                        {
                            const auto previous = std::exchange(f, 'O');
                            if(0 == solve(m, w, _x, _y, 2))
                                std::cout << "in loop " << _x << ' ' << _y + std::ranges::distance(v.begin(), std::ranges::find(v, 'O'))  << '\n';
                            f = previous;
                        }
                    return go(v, _x, _y, 2);
                }},
        GoToFun{[&m, w](const std::size_t _x, const std::size_t _y, const bool try_obstacles) // West
                {
                    const auto v = m | drop(_y * w) | take(_x + 1) | reverse;
                    if (try_obstacles)
                        for (auto &f : v | drop(1) | std::views::take_while([](auto c) { return c != '#'; }))
                        {
                            const auto previous = std::exchange(f, 'O');
                            if(0 == solve(m, w, _x, _y, 3))
                                std::cout << "in loop " << _x - std::ranges::distance(v.begin(), std::ranges::find(v, 'O')) << ' ' << _y << '\n';
                            f = previous;
                        }
                    return go(v, _x, _y, 3);
                }},
    };

    for (auto is_off_map = false, in_loop = false; !is_off_map && !in_loop; (++direction) %= dir_char.size())
    {
        std::tie(x, y, is_off_map, in_loop) = goto_functions[direction](x, y, try_obstacles);
        if(in_loop)
            return std::size_t{};
    }


    return std::ranges::count(m, 'X');
}

int main()
{
    static constexpr auto expected = size_t{1770};
    const auto result = std::apply(solve, read_map_from("day06.txt"));
    const auto is_equal = expected == result;
    // printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
