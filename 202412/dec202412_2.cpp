//
// Advent of Code 2024, day 12, part two
//

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <print>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);
		return (17 * 37 + h1) * 37 + h2;
	}
};

auto read_and_pad_map()
{
	std::vector<std::string> map;

	// Pad map with '.'
	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		map.push_back("." + line + ".");
	}

	map.insert(map.begin(), std::string(map.front().size(), '.'));
	map.push_back(std::string(map.front().size(), '.'));

	return map;
}

auto count_sides(const auto &seen)
{
	const auto [x_min, x_max] = std::ranges::minmax(std::views::elements<0>(seen));
	const auto [y_min, y_max] = std::ranges::minmax(std::views::elements<1>(seen));

	int sides = 0;

	// Scan top to bottom
	for (int y = y_min; y <= y_max + 1; ++y) {
		auto prev = std::make_pair(false, false);

		for (int x = x_min; x <= x_max + 1; ++x) {
			auto cur = std::make_pair(seen.contains({x, y - 1}),
			                          seen.contains({x, y}));

			// If there is a line segment here, count if different from
			// previous position (i.e. it is not a continued line)
			if (cur.first != cur.second) {
				sides += cur != prev;
			}

			prev = cur;
		}
	}

	// Scan left to right
	for (int x = x_min; x <= x_max + 1; ++x) {
		auto prev = std::make_pair(false, false);

		for (int y = y_min; y <= y_max + 1; ++y) {
			auto cur = std::make_pair(seen.contains({x - 1, y}),
			                          seen.contains({x, y}));

			// If there is a line segment here, count if different from
			// previous position (i.e. it is not a continued line)
			if (cur.first != cur.second) {
				sides += cur != prev;
			}

			prev = cur;
		}
	}

	return sides;
}

auto count_region(auto &map, int start_x, int start_y)
{
	constexpr std::array<std::array<int, 2>, 4> directions = {{
		{ 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 }
	}};

	std::unordered_set<std::pair<int, int>, PairHash> seen;

	std::queue<std::pair<int, int>> queue;

	queue.emplace(start_x, start_y);

	const auto plant = map[start_y][start_x];

	int area = 0;

	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		if (map[y][x] != plant) {
			continue;
		}

		if (auto [_, success] = seen.emplace(x, y); !success) {
			continue;
		}

		++area;

		for (auto [dx, dy] : directions) {
			queue.emplace(x + dx, y + dy);
		}
	}

	// Remove area from map
	for (auto [x, y] : seen) {
		map[y][x] = '.';
	}

	return std::make_pair(area, count_sides(seen));
}

int main()
{
	auto map = read_and_pad_map();

	long total_price = 0;

	for (const auto &[y, row] : std::views::enumerate(map)) {
		for (const auto &[x, plant] : std::views::enumerate(row)) {
			if (plant >= 'A' && plant <= 'Z') {
				auto [area, perimeter] = count_region(map, x, y);

				total_price += area * perimeter;
			}
		}
	}

	std::println("total price {}", total_price);
}
