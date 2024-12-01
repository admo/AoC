#include <algorithm>
#include <cstdio>
#include <fstream>
#include <numeric>
#include <ranges>
#include <unordered_map>
#include <vector>

template <typename T>
auto read_file(const std::string &filename)
{
    std::ifstream file(filename);
    std::vector<T> a;
    std::unordered_map<T, std::size_t> b;
    for (T x, y; file >> x >> y;)
    {
        a.push_back(x);
        ++b[y];
    }

    return std::make_pair(std::move(a), std::move(b));
}

int main()
{
    auto [a, b] = read_file<std::size_t>("day01.txt");

    for (auto &id : a)
        id *= b[id];

    constexpr auto expected = std::size_t{27732508};
    const auto result = std::reduce(a.begin(), a.end());
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
