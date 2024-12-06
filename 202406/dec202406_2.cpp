//
// Advent of Code 2024, day 6, part two
//

#include <cstddef>
#include <functional>
#include <iostream>
#include <print>
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

	// Pad map with 'E'
	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		map.push_back("E" + line + "E");
	}

	map.insert(map.begin(), std::string(map.front().size(), 'E'));
	map.push_back(std::string(map.front().size(), 'E'));

	return map;
}

bool guard_loops(const auto &map, int start_x, int start_y)
{
	int steps = 0;

	for (int x = start_x, y = start_y, dx = 0, dy = -1; ;) {
		while (map[y + dy][x + dx] == '#') {
			// Rotate direction 90 degrees right
			dy = std::exchange(dx, -dy);
		}

		x += dx;
		y += dy;

		if (map[y][x] == 'E') {
			break;
		}

		++steps;

		// If guard has walked 10.000 steps, assume he is looping
		//
		// I initially wrote this as a quick hack before writing
		// a version that stores visited positions and directions
		// in a set to know exactly when the guard starts looping.
		// However it turned out for this problem size it was an
		// order of magnitude faster to just check for a large
		// number of steps.
		if (steps == 10'000) {
			return true;
		}
	}

	return false;
}

int main()
{
	auto map = read_and_pad_map();

	auto [start_x, start_y] = [&map]() -> std::pair<int, int> {
		for (auto [y, str] : std::views::enumerate(map)) {
			if (auto x = str.find('^'); x != std::string::npos) {
				return {x, y};
			}
		}
		return {-1, -1};
	}();

	std::unordered_set<std::pair<int, int>, PairHash> visited;

	// Get positions visited by the guard
	for (int x = start_x, y = start_y, dx = 0, dy = -1; ;) {
		visited.emplace(x, y);

		while (map[y + dy][x + dx] == '#') {
			// Rotate direction 90 degrees right
			dy = std::exchange(dx, -dy);
		}

		x += dx;
		y += dy;

		if (map[y][x] == 'E') {
			break;
		}
	}

	visited.erase({start_x, start_y});

	int num_loop_positions = 0;

	// Try putting an obstacle in each position the guard visited
	for (auto [x, y] : visited) {
		map[y][x] = '#';

		if (guard_loops(map, start_x, start_y)) {
			++num_loop_positions;
		}

		map[y][x] = '.';
	}

	std::println("{} positions", num_loop_positions);
}
