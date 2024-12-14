//
// Advent of Code 2024, day 14, part two
//

#include <algorithm>
#include <array>
#include <cstdio>
#include <print>
#include <string>
#include <utility>
#include <vector>

struct Robot {
	std::pair<int, int> pos;
	std::pair<int, int> vel;
};

auto read_robots()
{
	std::vector<Robot> robots;

	Robot r = {};

	while (std::scanf("p=%d,%d v=%d,%d\n",
	                  &r.pos.first, &r.pos.second,
			  &r.vel.first, &r.vel.second) == 4) {
		robots.push_back(r);
	}

	return robots;
}

void step_robots(auto &robots, int width, int height)
{
	for (auto &robot : robots) {
		auto &[x, y] = robot.pos;
		auto [dx, dy] = robot.vel;

		x += dx;
		y += dy;

		if (x < 0) {
			x += width;
		}
		else if (x >= width) {
			x -= width;
		}

		if (y < 0) {
			y += height;
		}
		else if (y >= height) {
			y -= height;
		}
	}
}

void print_map(const auto &robots)
{
	std::array<std::string, 103> map;

	for (auto &line : map) {
		line.assign(101, '.');
	}

	for (const auto &robot : robots) {
		auto [x, y] = robot.pos;

		map[y][x] = '*';
	}

	for (const auto &line : map) {
		std::println("{}", line);
	}
}

bool has_line(auto &robots)
{
	std::ranges::sort(robots,
		[](const auto &lhs, const auto &rhs) {
			return lhs.pos < rhs.pos;
		}
	);

	int num_in_a_row = 0;
	int prev_y = -1;

	// Look for 20 robots in a line
	for (const auto &robot : robots) {
		auto [x, y] = robot.pos;

		if (y == prev_y + 1) {
			++num_in_a_row;

			if (num_in_a_row == 20) {
				return true;
			}
		}
		else {
			num_in_a_row = 0;
		}

		prev_y = y;
	}

	return false;
}

int main()
{
	auto robots = read_robots();

	std::println("read {} robots", robots.size());

	for (long seconds = 1; ; ++seconds) {
		step_robots(robots, 101, 103);

		if (has_line(robots)) {
			std::println("at {} seconds", seconds);
			print_map(robots);
			break;
		}

	}
}
