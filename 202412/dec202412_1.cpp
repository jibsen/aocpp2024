//
// Advent of Code 2024, day 12, part one
//

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
	int perimeter = 0;

	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		if (map[y][x] != plant) {
			++perimeter;
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

	return std::make_pair(area, perimeter);
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
