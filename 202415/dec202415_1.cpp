//
// Advent of Code 2024, day 15, part one
//

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

auto read_map()
{
	std::vector<std::string> map;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		map.push_back(std::move(line));
	}

	return map;
}

auto read_moves()
{
	std::string moves;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		moves.append(line);
	}

	return moves;
}

void print_map(const auto &map)
{
	std::ranges::copy(map, std::ostream_iterator<std::string>(std::cout, "\n"));
}

auto gps_sum(const auto &map)
{
	long sum = 0;

	for (const auto &[y, row] : std::views::enumerate(map)) {
		for (const auto &[x, ch] : std::views::enumerate(row)) {
			if (ch == 'O') {
				sum += y * 100 + x;
			}
		}
	}

	return sum;
}

int main()
{
	auto map = read_map();

	std::println("map {}x{}", map.front().size(), map.size());

	auto moves = read_moves();

	std::println("moves {}", moves.size());

	// Find robot
	auto [x, y] = [&map]() -> std::pair<int, int> {
		for (const auto &[y, str] : std::views::enumerate(map)) {
			if (auto x = str.find('@'); x != std::string::npos) {
				return {x, y};
			}
		}
		return {-1, -1};
	}();

	for (auto move : moves) {
		const auto [dx, dy] = [&move]() -> std::pair<int, int> {
			switch (move) {
			case '^':
				return {0, -1};
			case '>':
				return {1, 0};
			case 'v':
				return {0, 1};
			case '<':
				return {-1, 0};
			default:
				std::println(std::cerr, "bad move '{}'", move);
				std::exit(1);
			}
		}();

		auto box_x = x;
		auto box_y = y;

		// Skip over any boxes
		while (map[box_y + dy][box_x + dx] == 'O') {
			box_x += dx;
			box_y += dy;
		}

		// If there is a wall at the end, move is blocked
		if (map[box_y + dy][box_x + dx] == '#') {
			continue;
		}

		// Any line of boxes is moved here by writing a box after
		// the end and overwriting the first with the robot
		map[y][x] = '.';
		map[box_y + dy][box_x + dx] = 'O';
		x += dx;
		y += dy;
		map[y][x] = '@';
	}

	print_map(map);

	std::println("GPS sum {}", gps_sum(map));
}
