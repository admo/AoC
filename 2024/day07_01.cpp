#include <algorithm>
#include <bitset>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

auto read_equations_from(const char *str)
{
    auto result = std::size_t{};

    auto ifs = std::ifstream{str};
    for (std::string line; std::getline(ifs, line);)
    {
        auto iss = std::istringstream{line};
        std::size_t equation_result;
        iss >> equation_result;
        iss.ignore(2, ' ');

        std::vector<std::size_t> ingredients;
        std::copy(std::istream_iterator<std::size_t>(iss), std::istream_iterator<std::size_t>{},
                  std::back_inserter(ingredients));

        for (auto i = std::size_t{1} << ingredients.size(); i > 0; --i)
        {
            auto j = i;
            const auto proposed_result = std::ranges::fold_left(
                ingredients, 0, [&j](auto a, auto b) { return (std::exchange(j, j >> 1) & 1) ? a + b : a * b; });

            if (equation_result == proposed_result)
            {
                result += equation_result;
                break;
            }
        }
    }

    return result;
}
int main()
{
    static constexpr auto expected = size_t{12553187650171};
    const auto result = read_equations_from("day07.txt");
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
