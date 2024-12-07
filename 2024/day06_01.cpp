#include <algorithm>
#include <cstdio>
#include <fstream>
#include <functional>
#include <ranges>
#include <string>
#include <string_view>

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
    return std::make_tuple(std::move(map), width, x, y, dir_char.find(map[p]));
}

template <typename V>
auto go(V view, const std::size_t x, const std::size_t y, const std::size_t d)
{
    const auto p = std::ranges::find(view, '#');
    std::ranges::fill(view.begin(), p, 'X');
    const auto distance = std::distance(view.begin(), p) - 1;
    return std::make_tuple(x + dir_mod[d][0] * distance, y + dir_mod[d][1] * distance, p == view.end());
}

auto solve(std::string m, const std::size_t w, std::size_t x, std::size_t y, std::size_t direction)
{
    using GoToFun = std::function<std::tuple<std::size_t, std::size_t, bool>(std::size_t, std::size_t)>;

    const auto goto_functions = std::array{
        GoToFun{[&m, w](const std::size_t _x, const std::size_t _y) // Nort
                { return go(m | take(_y * w + _x + 1) | drop(_x) | stride(w) | reverse, _x, _y, 0); }},
        GoToFun{[&m, w](const std::size_t _x, const std::size_t _y) // East
                { return go(m | drop(_y * w + _x) | take(w - _x), _x, _y, 1); }},
        GoToFun{[&m, w](const std::size_t _x, const std::size_t _y) // South
                { return go(m | drop(_y * w + _x) | stride(w), _x, _y, 2); }},
        GoToFun{[&m, w](const std::size_t _x, const std::size_t _y) // West
                { return go(m | drop(_y * w) | take(_x + 1) | reverse, _x, _y, 3); }},
    };

    for (auto is_off_map = false; !is_off_map; (++direction) %= dir_char.size())
        std::tie(x, y, is_off_map) = goto_functions[direction](x, y);

    return std::ranges::count(m, 'X');
}

int main()
{
    static constexpr auto expected = size_t{5086};
    const auto result = std::apply(solve, read_map_from("day06.txt"));
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
