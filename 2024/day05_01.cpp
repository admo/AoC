#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Rules = std::unordered_map<std::size_t, std::vector<std::size_t>>;
using Updates = std::vector<std::vector<std::size_t>>;

auto read_file(const std::string &filename)
{
    auto file = std::ifstream{filename};
    auto rules = Rules{};

    for (std::string line; std::getline(file, line) && !line.empty();)
    {
        std::istringstream iss{line};
        std::size_t page1, page2;
        iss >> page1;
        iss.ignore(1);
        iss >> page2;
        rules[page1].push_back(page2);
    }

    auto updates = Updates{};

    for (std::string line; std::getline(file, line);)
    {
        auto update = std::vector<std::size_t>{};
        std::size_t page;
        for (std::istringstream iss{line}; iss >> page; iss.ignore(1))
        {
            update.push_back(page);
        }

        updates.push_back(std::move(update));
    }

    return std::make_pair(std::move(rules), std::move(updates));
}

auto solve(const Rules &rules, const Updates &updates)
{
    auto result = std::size_t{};

    for (const auto &update : updates)
    {
        auto is_correct = true;
        for (auto [i, p] : update | std::views::enumerate)
        {
            auto previous = update | std::views::take(i);
            is_correct &= std::ranges::find_first_of(previous, rules.at(p)) == previous.end();

            if (!is_correct)
                break;
        }

        result += is_correct ? update[update.size() / 2] : 0;
    }

    return result;
}

int main()
{
    static constexpr auto expected = size_t{5268};
    const auto [rules, updates] = read_file("day05.txt");
    const auto result = solve(rules, updates);
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
