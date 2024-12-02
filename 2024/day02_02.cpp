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

auto is_report_safe(const std::vector<int> &report)
{
    for (auto i = std::size_t{}; i < report.size(); ++i)
    {
        auto skip_ith = [i](const auto &e) { return std::get<0>(e) != i; };
        auto filtered_report = report | std::views::enumerate | std::views::filter(skip_ith) | std::views::values;

        const auto is_increasing = std::ranges::is_sorted(filtered_report, std::less<>{});
        const auto is_decreasing = std::ranges::is_sorted(filtered_report, std::greater<>{});

        if (!is_increasing && !is_decreasing)
            continue;

        std::vector<int> distances;
        distances.reserve(report.size());
        std::adjacent_difference(filtered_report.begin(), filtered_report.end(), std::back_inserter(distances),
                                 [](const int a, const int b) { return std::abs(a - b); });

        if (std::ranges::all_of(distances | std::views::drop(1),
                                [](const auto distance) { return distance > 0 && distance < 4; }))
            return true;
    }

    return false;
}

auto count_safe_reports(const std::string &filename)
{
    return std::ranges::count_if(read_reports_from_file(filename), &is_report_safe);
}

int main()
{
    constexpr auto expected = size_t{476};
    const auto result = count_safe_reports("day02.txt");
    const auto is_equal = expected == result;
    printf("%lu == %ld -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
