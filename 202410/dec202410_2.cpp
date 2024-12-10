//
// Advent of Code 2024, day 10, part two
//

#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <print>
#include <queue>
#include <ranges>
#include <string>
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

auto get_trailheads(const auto &map)
{
	std::vector<std::pair<int, int>> trailheads;

	for (const auto &[y, row] : std::views::enumerate(map)) {
		for (const auto &[x, height] : std::views::enumerate(row)) {
			if (height == '0') {
				trailheads.emplace_back(x, y);
			}
		}
	}

	return trailheads;
}

auto trailhead_rating(const auto &map, int start_x, int start_y)
{
	constexpr std::array<std::array<int, 2>, 4> directions = {{
		{ 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 }
	}};

	std::queue<std::pair<int, int>> queue;

	queue.emplace(start_x, start_y);

	int rating = 0;

	while (!queue.empty()) {
		auto [x, y] = queue.front();
		queue.pop();

		if (map[y][x] == '9') {
			++rating;
			continue;
		}

		for (auto [dx, dy] : directions) {
			if (map[y + dy][x + dx] == map[y][x] + 1) {
				queue.emplace(x + dx, y + dy);
			}
		}
	}

	return rating;
}

int main()
{
	auto map = read_and_pad_map();

	auto trailheads = get_trailheads(map);

	int trailhead_rating_sum = 0;

	for (auto [x, y] : trailheads) {
		trailhead_rating_sum += trailhead_rating(map, x, y);
	}

	std::println("trailhead rating sum {}", trailhead_rating_sum);
}
