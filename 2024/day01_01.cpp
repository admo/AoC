#include <algorithm>
#include <cstdio>
#include <fstream>
#include <numeric>
#include <ranges>
#include <vector>

template <typename T>
auto read_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<T> a, b;
    for (T x, y; file >> x >> y;)
    {
        a.push_back(x);
        b.push_back(y);
    }

    return std::make_pair(std::move(a), std::move(b));
}

int main()
{
    auto [a, b] = read_file<long>("day01.txt");

    std::ranges::sort(a);
    std::ranges::sort(b);

    const auto sum_view = std::views::zip_transform([](auto x, auto y) { return std::abs(x - y); }, a, b);

    constexpr auto expected = long{765748};
    const auto result = std::reduce(sum_view.begin(), sum_view.end());
    const auto is_equal = expected == result;
    std::printf("%ld == %ld -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
