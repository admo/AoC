#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <ranges>
#include <utility>
#include <vector>

using std::ranges::transform;
using std::views::drop;
using std::views::enumerate;
using std::views::iota;
using std::views::istream;
using std::views::reverse;
using std::views::stride;
using std::views::zip;

auto read_disk_map_from(const char *filename)
{
    auto disk_map = std::ifstream{filename};
    auto disk = std::vector<std::size_t>{};

    transform(istream<char>(disk_map), std::back_inserter(disk),
              [](const std::uint8_t c) { return static_cast<std::size_t>(c - '0'); });

    auto block_ids = std::vector<std::size_t>(disk.size());
    std::exclusive_scan(disk.begin(), disk.end(), block_ids.begin(), 0);

    return std::make_pair(std::move(disk), std::move(block_ids));
}

auto file_checksum(const std::size_t block_id, const std::size_t file_id, const std::size_t file_size)
{
    // sum of n elements of arithmetic progression
    const auto series_sum = file_size * (2 * block_id + file_size - 1) / 2;
    return file_id * series_sum;
}

int main()
{
    auto [disk_map, block_ids] = read_disk_map_from("day09.txt");

    auto front_files = zip(iota(0), disk_map | stride(2), block_ids | stride(2));
    auto gaps = zip(iota(0), disk_map | drop(1) | stride(2), block_ids | drop(1) | stride(2));
    auto back_files = front_files | reverse;
    auto back_files_it = back_files.begin();

    auto result = std::size_t{};

    for (auto [gap_id, gap_size, block_id] : gaps)
    {
        while (gap_size && back_files_it != back_files.end() && gap_id < std::get<0>(*back_files_it))
        {
            auto [file_id, file_size, _] = *back_files_it;
            const auto moved_size = std::min(gap_size, file_size);
            result += file_checksum(block_id, file_id, moved_size);
            file_size -= moved_size;
            gap_size -= moved_size;
            block_id += moved_size;

            if (!file_size)
                ++back_files_it;
        }
    }

    for (auto [file_id, file_size, block_id] : front_files)
    {
        result += file_checksum(block_id, file_id, file_size);
    }

    static constexpr auto expected = size_t{6154342787400};
    const auto is_equal = expected == result;
    printf("%zu == %zu -> %s\n", expected, result, is_equal ? "true" : "false");

    return is_equal ? 0 : 1;
}
