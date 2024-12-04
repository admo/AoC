#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

auto read_word_search_puzzle(const std::string &filename)
{
    std::ifstream file{filename};
    std::vector<std::string> reports;

    for (std::string line; std::getline(file, line);)
    {
        reports.push_back(std::move(line));
    }

    return reports;
}

auto find_xmas(const std::vector<std::string> &puzzle, const int x, const int y, const int dx, const int dy)
{
    const auto width = puzzle.front().size();
    const auto height = puzzle.size();
    static constexpr auto word = std::string_view{"XMAS"};

    for (auto i = int{0}; i < word.size(); ++i)
    {
        const auto nx = x + i * dx;
        const auto ny = y + i * dy;

        if (nx >= width || ny >= height || nx < 0 || ny < 0)
            return false;

        if (puzzle[ny][nx] != word[i])
            return false;
    }

    return true;
}

auto count_xmas_at(const std::vector<std::string> &puzzle, const int x, const int y)
{
    auto result = std::size_t{0};

    for (const auto dx : {-1, 0, 1})
        for (const auto dy : {-1, 0, 1})
            result += find_xmas(puzzle, x, y, dx, dy);

    return result;
}

auto solve(const std::vector<std::string> &puzzle)
{
    std::size_t result = 0;

    const auto width = puzzle.front().size();
    const auto height = puzzle.size();

    for (int x = 0; x < width; ++x)
    {
        for (int y = 0; y < height; ++y)
        {
            result += count_xmas_at(puzzle, x, y);
        }
    }

    return result;
}

int main()
{
    static constexpr auto expected = int{2401};
    const auto puzzle = read_word_search_puzzle("day04.txt");
    const auto result = solve(puzzle);
    const auto is_equal = expected == result;
    printf("%d == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
