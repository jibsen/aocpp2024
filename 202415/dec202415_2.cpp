//
// Advent of Code 2024, day 15, part two
//

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <print>
#include <queue>
#include <ranges>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

auto expand_line(const std::string &line)
{
	std::string expanded_line;

	expanded_line.reserve(2 * line.size());

	for (auto ch : line) {
		switch (ch) {
		case '#':
			expanded_line.append("##");
			break;
		case 'O':
			expanded_line.append("[]");
			break;
		case '.':
			expanded_line.append("..");
			break;
		case '@':
			expanded_line.append("@.");
			break;
		default:
			std::println(std::cerr, "bad map character '{}'", ch);
			std::exit(1);
		}
	}

	return expanded_line;
}

auto read_map()
{
	std::vector<std::string> map;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		map.push_back(expand_line(line));
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
			if (ch == '[') {
				sum += y * 100 + x;
			}
		}
	}

	return sum;
}

auto get_characters_to_move(const auto &map, int x, int y, int dx, int dy)
{
	std::vector<std::tuple<int, int, char>> box_chars;

	std::queue<std::pair<int, int>> queue;

	auto add_box = [&](int x, int y, int dy) {
		// dbox is the extra offset in the x direction to get the
		// other character of the box
		auto dbox = map[y][x] == '[' ? 1 : -1;

		// If we come from above or below, we have to handle both
		// characters of the box, whereas if you come from the left
		// or right, we only need to handle the other character of
		// the box in the direction we're moving
		if (dy != 0) {
			queue.emplace(x, y);
		}
		queue.emplace(x + dbox, y);

		// Store both characters of the box so we can move them later
		box_chars.emplace_back(x, y, map[y][x]);
		box_chars.emplace_back(x + dbox, y, map[y][x + dbox]);
	};

	if (map[y + dy][x + dx] != '[' && map[y + dy][x + dx] != ']') {
		return box_chars;
	}

	add_box(x + dx, y + dy, dy);

	bool blocked = false;

	while (!queue.empty()) {
		auto [box_x, box_y] = queue.front();
		queue.pop();

		// If there is a wall in front of any of the boxes we are
		// moving, the entire move is blocked
		if (map[box_y + dy][box_x + dx] == '#') {
			blocked = true;
			break;
		}

		if (map[box_y + dy][box_x + dx] == '.') {
			continue;
		}

		// Here, there is a part of another box in front of us
		add_box(box_x + dx, box_y + dy, dy);
	}

	if (blocked) {
		box_chars.clear();
	}

	return box_chars;
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

		// Check if immediately blocked
		if (map[y + dy][x + dx] == '#') {
			continue;
		}

		// Check if simple move
		if (map[y + dy][x + dx] == '.') {
			map[y][x] = '.';
			x += dx;
			y += dy;
			map[y][x] = '@';
			continue;
		}

		// Here there is a box in front of the robot, get all
		// characters belonging to boxes to be moved
		auto box_chars = get_characters_to_move(map, x, y, dx, dy);

		if (box_chars.empty()) {
			continue;
		}

		// Remove all characters belonging to boxes to be moved
		for (auto [bx, by, _] : box_chars) {
			map[by][bx] = '.';
		}

		// Add moved characters for boxes
		for (auto [bx, by, ch] : box_chars) {
			map[by + dy][bx + dx] = ch;
		}

		// Move robot
		map[y][x] = '.';
		x += dx;
		y += dy;
		map[y][x] = '@';
	}

	print_map(map);

	std::println("GPS sum {}", gps_sum(map));
}
