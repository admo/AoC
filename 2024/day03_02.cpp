#include <array>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>

enum class InstructionType
{
    Mul,
    Do,
    Dont
};

inline constexpr auto instruction_set = std::array<std::string_view, 3>{"mul(", "do()", "don't()"};

auto find_instruction(const std::string &line, const std::size_t from = 0)
{
    auto result = std::make_pair(std::string::npos, InstructionType::Dont);

    for (std::size_t i = 0; i < instruction_set.size(); ++i)
    {
        if (const auto pos = line.find(instruction_set[i], from); pos < result.first)
        {
            result.first = pos;
            result.second = static_cast<InstructionType>(i);
        }
    }

    return result;
}

auto execute_mul(const std::string &line, std::size_t p)
{
    static constexpr auto numbers = std::string_view{"0123456789"};
    const auto n1_pos = p + instruction_set[0].size();

    p = line.find_first_not_of(numbers, n1_pos);
    if (p == std::string::npos || line[p] != ',' || p == n1_pos)
        return std::make_pair(long{}, p);

    const auto n2_pos = p + 1;

    p = line.find_first_not_of(numbers, n2_pos);
    if (p == std::string::npos || line[p] != ')' || p == n2_pos)
        return std::make_pair(long{}, p);

    p += 1;

    const auto first_number = std::strtol(line.data() + n1_pos, nullptr, 10);
    const auto second_number = std::strtol(line.data() + n2_pos, nullptr, 10);

    return std::make_pair(first_number * second_number, p);
}

auto run_program_from(const std::string &filename)
{
    std::ifstream file{filename};

    long result = 0;
    long partial_result;
    bool execute_instruction = true;

    for (std::string line; std::getline(file, line);)
    {
        for (auto [p, i] = find_instruction(line); p != std::string::npos; std::tie(p, i) = find_instruction(line, p))
        {
            switch (i)
            {
            case InstructionType::Do:
                p += instruction_set[1].size();
                execute_instruction = true;
                break;
            case InstructionType::Dont:
                p += instruction_set[2].size();
                execute_instruction = false;
                break;
            case InstructionType::Mul:
                std::tie(partial_result, p) = execute_mul(line, p);
                result += execute_instruction ? partial_result : 0;
            }
        }
    }

    return result;
}

int main()
{
    constexpr auto expected = long{74361272};
    const auto result = run_program_from("day03.txt");
    const auto is_equal = expected == result;
    printf("%ld == %ld -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
