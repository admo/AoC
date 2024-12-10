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

void left_antinodes(const std::string &map, std::string &antinodes, std::size_t a1, const std::size_t d,
                    const std::size_t br)
{
    antinodes[a1] = '#';

    for (; a1 >= d && count_line_breaks(map, a1 - d, d) == br && map[a1 - d] != '\n'; a1 -= d)
    {
        antinodes[a1 - d] = '#';
    }
}

void right_antinodes(const std::string &map, std::string &antinodes, std::size_t a2, const std::size_t d,
                     const std::size_t br)
{
    antinodes[a2] = '#';

    for (; a2 < map.size() && count_line_breaks(map, a2, d) == br && map[a2 + d] != '\n'; a2 += d)
    {
        antinodes[a2 + d] = '#';
    }
}

auto solve(const std::string &map)
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

            left_antinodes(map, antinodes, a1, distance, line_breaks);
            right_antinodes(map, antinodes, a2, distance, line_breaks);
        }
    }

    return std::ranges::count(antinodes, '#');
}

int main()
{
    static constexpr auto expected = size_t{1333};
    const auto result = solve(read_map_from("day08.txt"));
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
