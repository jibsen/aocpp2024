//
// Advent of Code 2024, day 20, part two
//

#include <array>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <print>
#include <queue>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>
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

auto get_coords_of(const auto &map, char ch) -> std::pair<int, int>
{
	for (const auto &[y, str] : std::views::enumerate(map)) {
		if (auto x = str.find(ch); x != std::string::npos) {
			return {x, y};
		}
	}

	return {-1, -1};
}

auto create_distance_lookup(const auto &map, int start_x, int start_y)
{
	constexpr std::array<std::array<int, 2>, 4> directions = {{
		{ 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 }
	}};

	std::unordered_map<std::pair<int, int>, int, PairHash> lookup;

	std::queue<std::tuple<int, int, int>> queue;

	queue.emplace(start_x, start_y, 0);

	while (!queue.empty()) {
		auto [x, y, dist] = queue.front();
		queue.pop();

		if (auto [it, success] = lookup.insert({{x, y}, dist}); !success) {
			continue;
		}

		for (auto [dx, dy] : directions) {
			if (map[y + dy][x + dx] != '#') {
				queue.emplace(x + dx, y + dy, dist + 1);
			}
		}
	}

	return lookup;
}

auto get_reachable_within_distance(const auto &map, int x, int y, int max_dist)
{
	auto inside_map = [&](int x, int y) {
		return x >= 0 && x < map.front().size()
		    && y >= 0 && y < map.size();
	};

	std::vector<std::tuple<int, int, int>> reachable;

	// Add all non-wall positions within taxicab distance max_dist
	for (int dy = -max_dist; dy <= max_dist; ++dy) {
		for (int dx = -max_dist; dx <= max_dist; ++dx) {
			auto dist = std::abs(dx) + std::abs(dy);

			if (dist > 1 && dist <= max_dist
			 && inside_map(x + dx, y + dy)
			 && map[y + dy][x + dx] != '#') {
				reachable.emplace_back(x + dx, y + dy, dist);
			}
		}
	}

	return reachable;
}

int main()
{
	auto map = read_map();

	std::println("map {}x{}", map.front().size(), map.size());

	auto [start_x, start_y] = get_coords_of(map, 'S');
	auto [end_x, end_y] = get_coords_of(map, 'E');

	auto dist_from_start = create_distance_lookup(map, start_x, start_y);
	auto dist_to_end = create_distance_lookup(map, end_x, end_y);

	auto fastest_time = dist_from_start[{end_x, end_y}];

	std::println("fastest time {}", fastest_time);

	long num_saving_100 = 0;

	for (auto [pos, dist_to_pos] : dist_from_start) {
		auto [x, y] = pos;

		auto reachable = get_reachable_within_distance(map, x, y, 20);

		for (auto [cheat_x, cheat_y, cheat_dist] : reachable) {
			if (auto end_it = dist_to_end.find({cheat_x, cheat_y}); end_it != dist_to_end.end()) {
				auto new_time = dist_to_pos + end_it->second + cheat_dist;

				if (new_time + 100 <= fastest_time) {
					++num_saving_100;
				}
			}
		}
	}

	std::println("{} cheats save at least 100", num_saving_100);
}
