#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ranges>
#include <utility>
#include <vector>

using std::views::reverse;
using std::views::stride;
using std::views::drop;
using std::views::enumerate;
using std::ranges::find_if;

auto read_disk_map_from(const char *filename)
{
    auto disk_map = std::ifstream{filename};
    auto disk = std::vector<std::size_t>{};

    std::transform(std::istream_iterator<char>{disk_map}, {}, std::back_inserter(disk),
                   [](const std::uint8_t c) { return static_cast<std::size_t>(c - '0'); });

    return disk;
}

auto file_checksum(const std::size_t block_id, const std::size_t file_id, const std::size_t file_size)
{
    // sum of n elements of arithmetic progression
    const auto series_sum = file_size * (2 * block_id + file_size - 1) / 2;
    return file_id * series_sum;
}

int main()
{
    static constexpr auto expected = size_t{6154342787400};

    auto disk_map = read_disk_map_from("day09.txt");
    auto front_files = disk_map | stride(2) | enumerate;;
    auto back_files = front_files | reverse;
    auto back_files_it = back_files.begin();
    auto gaps = disk_map | drop(1) | stride(2);

    auto block_id = std::size_t{};
    auto result = std::size_t{};

    for (auto [file_id, file_size]: front_files)
    {
        // front file
        result += file_checksum(block_id, file_id, file_size);
        block_id += std::exchange(file_size, 0);

        // gap
        while (gaps[file_id] && back_files_it != back_files.end())
        {
            back_files_it = find_if(back_files_it, back_files.end(), [](const auto &x) { return std::get<1>(x); });

            if (back_files_it == back_files.end())
                break;

            auto [back_file_id, back_file_size] = *back_files_it;

            const auto ddd = std::min(gaps[file_id], back_file_size);
            result += file_checksum(block_id, back_file_id, ddd);
            back_file_size -= ddd;
            gaps[file_id] -= ddd;
            block_id += ddd;
        }
    }

    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
