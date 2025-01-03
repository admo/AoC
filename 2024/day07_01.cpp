#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using std::ranges::fold_left;

auto read_equations_from(const char *str)
{
    auto result = std::size_t{};

    auto ifs = std::ifstream{str};
    auto ingredients = std::vector<std::size_t>{};
    for (std::string line; std::getline(ifs, line); ingredients.clear())
    {
        auto iss = std::istringstream{line};
        std::size_t exptected_result;
        iss >> exptected_result;
        iss.ignore(2, ' ');

        std::copy(std::istream_iterator<std::size_t>(iss), std::istream_iterator<std::size_t>{},
                  std::back_inserter(ingredients));

        // op_combination is a bitset that will be used to generate all possible variations of operations
        const auto number_of_op_variations = static_cast<std::size_t>(std::pow(2, ingredients.size()));
        for (auto op_variation = std::size_t{}; op_variation < number_of_op_variations; ++op_variation)
        {
            auto i = op_variation;
            const auto proposed_result =
                fold_left(ingredients, std::size_t{}, [&i](auto a, auto b) { return std::exchange(i, i / 2) % 2 ? a + b : a * b; });

            if (exptected_result == proposed_result)
            {
                result += exptected_result;
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
