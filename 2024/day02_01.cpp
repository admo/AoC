#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>

bool is_distance_valid(const int a, const int b)
{
    const auto distance = std::abs(a - b);
    return distance > 0 && distance < 4;
}

auto read_file(const std::string &filename)
{
    std::ifstream file{filename};
    std::string line;
    int number_of_safe_reports = 0;

    while (std::getline(file, line))
    {
        std::istringstream iss{line};

        int previous_number, current_number;
        iss >> previous_number >> current_number;

        const auto is_increasing = previous_number < current_number;

        while (is_increasing == (previous_number < current_number) &&
               is_distance_valid(previous_number, current_number))
        {
            previous_number = current_number;

            if (!(iss >> current_number))
            {
                ++number_of_safe_reports;
                break;
            }
        }
    }

    return number_of_safe_reports;
}

int main()
{
    constexpr auto expected = int{421};
    const auto result = read_file("day02.txt");
    const auto is_equal = expected == result;
    printf("%d == %d -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
