#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>

auto run_program_from(const std::string &filename)
{
    std::ifstream file{filename};

    static constexpr auto instruction = std::string_view{"mul("};
    static constexpr auto numbers = std::string_view{"0123456789"};

    long result = 0;

    for (std::string line; std::getline(file, line);)
    {
        for (std::size_t p = line.find(instruction); p != std::string::npos; p = line.find(instruction, p))
        {
            const auto n1_pos = p + instruction.size();

            p = line.find_first_not_of(numbers, p + instruction.size());
            if (p == std::string::npos || line[p] != ',' || p == n1_pos)
                continue;

            const auto n2_pos = p + 1;

            p = line.find_first_not_of(numbers, n2_pos);
            if (p == std::string::npos || line[p] != ')' || p == n2_pos)
                continue;

            const auto first_number = std::strtol(line.data() + n1_pos, nullptr, 10);
            const auto second_number = std::strtol(line.data() + n2_pos, nullptr, 10);

            result += first_number * second_number;
        }
    }

    return result;
}

int main()
{
    constexpr auto expected = long{175615763};
    const auto result = run_program_from("day03.txt");
    const auto is_equal = expected == result;
    printf("%ld == %ld -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
