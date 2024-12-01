#include <algorithm>
#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

auto read_file(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<std::size_t> a, b;
    for(int x, y; file >> x >> y;)
    {
        a.push_back(x);
        b.push_back(y);
    }

    return std::make_pair(std::move(a), std::move(b));}

int main()
{
    auto [a, b] = read_file("day01.txt");

    for (auto& id: a)
        id *= std::ranges::count(b, id);

    std::puts(std::format("{}", std::reduce(a.begin(), a.end())).c_str());

    return 0;
}
