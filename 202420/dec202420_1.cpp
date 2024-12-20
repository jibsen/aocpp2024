//
// Advent of Code 2024, day 20, part one
//

#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <string>
#include <tuple>
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

auto read_map()
{
	std::vector<std::string> map;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		map.push_back(std::move(line));
	}

	return map;
}

int shortest_distance(const auto &map, int start_x, int start_y, int end_x, int end_y)
{
	constexpr std::array<std::array<int, 2>, 4> directions = {{
		{ 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 }
	}};

	std::unordered_set<std::pair<int, int>, PairHash> seen;

	std::queue<std::tuple<int, int, int>> queue;

	queue.emplace(start_x, start_y, 0);

	while (!queue.empty()) {
		auto [x, y, dist] = queue.front();
		queue.pop();

		if (x == end_x && y == end_y) {
			return dist;
		}

		if (auto [it, success] = seen.insert({x, y}); !success) {
			continue;
		}

		for (auto [dx, dy] : directions) {
			if (map[y + dy][x + dx] != '#') {
				queue.emplace(x + dx, y + dy, dist + 1);
			}
		}
	}

	return -1;
}

auto get_coords_of(const auto &map, char ch) -> std::pair<int, int>
{
	for (const auto &[y, str] : std::views::enumerate(map)) {
		if (auto x = str.find(ch); x != std::string::npos) {
			return {x, y};
		}
	}

	return {-1, -1};
}

int main()
{
	auto map = read_map();

	std::println("map {}x{}", map.front().size(), map.size());

	auto [start_x, start_y] = get_coords_of(map, 'S');
	auto [end_x, end_y] = get_coords_of(map, 'E');

	auto fastest_time = shortest_distance(map, start_x, start_y, end_x, end_y);

	std::println("fastest time {}", fastest_time);

	long num_saving_100 = 0;

	for (int y = 1; y + 1 < map.size(); ++y) {
		for (int x = 1; x + 1 < map.front().size(); ++x) {
			if (map[y][x] == '.') {
				continue;
			}

			if ((map[y - 1][x] != '#' && map[y + 1][x] != '#')
			 || (map[y][x - 1] != '#' && map[y][x + 1] != '#')) {
				map[y][x] = '.';

				auto distance = shortest_distance(map, start_x, start_y, x, y)
				              + shortest_distance(map, x, y, end_x, end_y);

				if (distance + 100 <= fastest_time) {
					++num_saving_100;
				}

				map[y][x] = '#';
			}
		}
	}

	std::println("{} cheats save at least 100", num_saving_100);
}
