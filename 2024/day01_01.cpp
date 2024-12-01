#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

auto read_file(const std::string& filename)
{
    std::ifstream file(filename);
    std::vector<int> a, b;
    for(int x, y; file >> x >> y;)
    {
        a.push_back(x);
        b.push_back(y);
    }

    return std::make_pair(std::move(a), std::move(b));}

int main()
{
    auto [a, b] = read_file("day01.txt");
    std::cout << a.size() << '\n';

    std::ranges::sort(a);
    std::ranges::sort(b);

    const auto sum_View = std::views::zip_transform([](auto x, auto y){ return std::abs(x-y); }, a, b);

    std::cout << std::reduce(sum_View.begin(), sum_View.end()) << '\n';

    return 0;
}
