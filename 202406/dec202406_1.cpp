//
// Advent of Code 2024, day 6, part one
//

#include <algorithm>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

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

int main()
{
	auto map = read_and_pad_map();

	auto [start_x, start_y] = [&map]() -> std::pair<int, int> {
		for (const auto &[y, str] : std::views::enumerate(map)) {
			if (auto x = str.find('^'); x != std::string::npos) {
				return {x, y};
			}
		}
		return {-1, -1};
	}();

	// Mark positions visited by the guard with X
	for (int x = start_x, y = start_y, dx = 0, dy = -1; ;) {
		map[y][x] = 'X';

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

	// Count number of Xs
	auto positions_visited = std::ranges::fold_left(
		map | std::views::transform(
			[](const auto &str) {
				return std::ranges::count(str, 'X');
			}
		),
		0,
		std::plus{}
	);

	std::println("{} positions visited", positions_visited);
}
