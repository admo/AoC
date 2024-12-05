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

using std::back_inserter;
using std::istream_iterator;
using std::size_t;
using std::ranges::copy;
using std::ranges::fold_left;
using std::ranges::replace;
using std::views::filter;
using std::views::transform;

auto read_file(const std::string &filename)
{
    auto file = std::ifstream{filename};

    auto rules = Rules{};
    auto updates = Updates{};

    for (std::string line; std::getline(file, line) && !line.empty();)
    {
        replace(line, '|', ' ');
        size_t page1, page2;
        std::istringstream{line} >> page1 >> page2;
        rules[page1].insert(page2);
    }

    for (std::string line; std::getline(file, line);)
    {
        replace(line, ',', ' ');
        auto iss = std::istringstream{line};
        copy(istream_iterator<size_t>{iss}, istream_iterator<size_t>{}, back_inserter(updates.emplace_back()));
    }

    return std::make_pair(std::move(rules), std::move(updates));
}

auto solve(const Rules &rules, const Updates &updates)
{
    auto is_in_order = [&rules](const auto a, const auto b) { return !rules.contains(a) || rules.at(a).contains(b); };
    auto is_valid = [&is_in_order](const auto &update) { return std::ranges::is_sorted(update, is_in_order); };
    auto get_middle = [](const auto &update) { return update[update.size() / 2]; };

    return fold_left(updates | filter(is_valid) | transform(get_middle), size_t{}, std::plus<>{});
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
