#include <algorithm>
#include <cstdio>
#include <fstream>
#include <numeric>
#include <ranges>
#include <vector>

template<typename T>
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
    auto [a, b] = read_file<std::size_t>("day01.txt");

    for (auto &id : a)
        id *= std::ranges::count(b, id);

    printf("%zu\n", std::reduce(a.begin(), a.end()));

    return 0;
}
