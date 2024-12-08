//
// Advent of Code 2024, day 8, part two
//

#include <algorithm>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

auto get_antennas(const auto &map)
{
	std::unordered_map<char, std::vector<std::pair<int, int>>> antennas;

	for (const auto &[y, line] : std::views::enumerate(map)) {
		for (const auto &[x, ch] : std::views::enumerate(line)) {
			if (ch != '.') {
				antennas[ch].emplace_back(x, y);
			}
		}
	}

	return antennas;
}

int main()
{
	std::vector<std::string> map(std::istream_iterator<std::string>{std::cin},
	                             std::istream_iterator<std::string>{});

	auto antennas = get_antennas(map);

	for (const auto &[_, positions] : antennas) {
		for (const auto &[lhs, rhs] : std::views::cartesian_product(positions, positions)) {
			if (lhs == rhs) {
				continue;
			}

			auto within_map = [&map](int x, int y) {
				return x >= 0 && x < std::ssize(map.front())
				    && y >= 0 && y < std::ssize(map);
			};

			auto [x1, y1] = lhs;
			auto [x2, y2] = rhs;

			auto dx = x2 - x1;
			auto dy = y2 - y1;

			for (auto x = x1, y = y1; within_map(x, y); x += dx, y += dy) {
				map[y][x] = '#';
			}

			for (auto x = x1, y = y1; within_map(x, y); x -= dx, y -= dy) {
				map[y][x] = '#';
			}
		}
	}

	// Count number of #s
	auto num_antinodes = std::ranges::count(map | std::views::join, '#');

	std::println("{} antinodes", num_antinodes);
}
