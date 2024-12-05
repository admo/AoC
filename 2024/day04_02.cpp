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

auto find_mas(const std::vector<std::string> &puzzle, const int x, const int y, const int dx, const int dy)
{
    static constexpr auto word = std::string_view{"MAS"};
    const auto width = puzzle.front().size();
    const auto height = puzzle.size();

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
    const auto mas1 = find_mas(puzzle, x - 1, y - 1, 1, 1) || find_mas(puzzle, x + 1, y + 1, -1, -1);
    const auto mas2 = find_mas(puzzle, x - 1, y + 1, 1, -1) || find_mas(puzzle, x + 1, y - 1, -1, 1);

    return mas1 && mas2 ? std::size_t{1} : std::size_t{0};
}

auto solve(const std::vector<std::string> &puzzle)
{
    std::size_t result = 0;

    const auto width = puzzle.front().size();
    const auto height = puzzle.size();

    for (int x = 1; x < width - 1; ++x)
    {
        for (int y = 1; y < height - 1; ++y)
        {
            result += count_xmas_at(puzzle, x, y);
        }
    }

    return result;
}

int main()
{
    static constexpr auto expected = size_t{1822};
    const auto puzzle = read_word_search_puzzle("day04.txt");
    const auto result = solve(puzzle);
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
