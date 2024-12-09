#include <algorithm>
#include <cstdio>
#include <fstream>
#include <functional>
#include <iostream>
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

auto read_map_from(const std::string &filename)
{
    auto file = std::ifstream{filename};
    return std::string{std::istreambuf_iterator{file}, std::istreambuf_iterator<char>{}};
}

auto count_line_breaks(const std::string &map, const std::size_t start, const std::size_t distance)
{

    return std::ranges::count(map | drop(start) | take(distance), '\n');
}

auto solve(std::string map)
{
    auto antinodes = std::string(map.size(), '.');
    for (auto a1 = map.find_first_not_of(".#\n", 0); a1 != std::string::npos;
         a1 = map.find_first_not_of(".#\n", a1 + 1))
    {
        for (auto a2 = map.find_first_of(map[a1], a1 + 1); a2 != std::string::npos;
             a2 = map.find_first_of(map[a1], a2 + 1))
        {
            const auto distance = a2 - a1;
            const auto line_breaks = count_line_breaks(map, a1, distance);

            if (a1 >= distance && count_line_breaks(map, a1 - distance, distance) == line_breaks &&
                map[a1 - distance] != '\n')
            {
                antinodes[a1 - distance] = '#';
            }
            if (a2 + distance < map.size() && count_line_breaks(map, a2, distance) == line_breaks &&
                map[a2 + distance] != '\n')
            {
                antinodes[a2 + distance] = '#';
            }
        }
    }
    return std::ranges::count(antinodes, '#');
}

int main()
{
    static constexpr auto expected = size_t{398};
    const auto result = solve(read_map_from("day08.txt"));
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
