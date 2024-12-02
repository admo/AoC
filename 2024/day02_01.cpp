#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <vector>

auto read_reports_from_file(const std::string &filename)
{
    std::ifstream file{filename};
    std::vector<std::vector<int>> reports;

    for (std::string line; std::getline(file, line);)
    {
        std::istringstream iss{line};
        std::vector<int> report;
        std::copy(std::istream_iterator<int>{iss}, std::istream_iterator<int>{}, std::back_inserter(report));

        reports.push_back(std::move(report));
    }

    return reports;
}

auto is_report_safe(std::vector<int> report)
{
    const auto is_increasing = std::ranges::is_sorted(report, std::less<>{});
    const auto is_decreasing = std::ranges::is_sorted(report, std::greater<>{});

    if (!is_increasing && !is_decreasing)
        return false;

    std::adjacent_difference(report.begin(), report.end(), report.begin(),
                             [](const int a, const int b) { return std::abs(a - b); });

    return std::all_of(report.begin() + 1, report.end(),
                       [](const auto distance) { return distance > 0 && distance < 4; });
}

auto count_safe_reports(const std::string &filename)
{
    return std::ranges::count_if(read_reports_from_file(filename), [](const auto &report) { return is_report_safe(report); });
}

int main()
{
    constexpr auto expected = size_t{421};
    const auto result = count_safe_reports("day02.txt");
    const auto is_equal = expected == result;
    printf("%lu == %ld -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
