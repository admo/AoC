#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Rules = std::unordered_map<std::size_t, std::set<std::size_t>>;
using Updates = std::vector<std::vector<std::size_t>>;

auto read_file(const std::string &filename)
{
    auto file = std::ifstream{filename};
    auto rules = Rules{};

    for (std::string line; std::getline(file, line) && !line.empty();)
    {
        std::ranges::replace(line, '|', ' ');
        std::istringstream iss{line};
        std::size_t page1, page2;
        iss >> page1 >> page2;
        rules[page1].insert(page2);
    }

    auto updates = Updates{};

    for (std::string line; std::getline(file, line);)
    {
        std::ranges::replace(line, ',', ' ');
        std::istringstream iss{line};
        std::ranges::copy(std::istream_iterator<std::size_t>{iss}, std::istream_iterator<std::size_t>{},
                          std::back_inserter(updates.emplace_back()));
    }

    return std::make_pair(std::move(rules), std::move(updates));
}

auto solve(const Rules &rules, Updates updates)
{
    auto result = std::size_t{};
    auto predicate = [&rules](const auto a, const auto b) { return !rules.contains(a) || rules.at(a).contains(b); };

    for (auto &update : updates)
    {
        if (std::ranges::is_sorted(update, predicate))
            continue;

        std::ranges::sort(update, predicate);
        result += update[update.size() / 2];
    }

    return result;
}

int main()
{
    static constexpr auto expected = size_t{5799};
    auto [rules, updates] = read_file("day05.txt");
    const auto result = solve(rules, std::move(updates));
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
