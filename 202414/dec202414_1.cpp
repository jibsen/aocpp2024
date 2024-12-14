//
// Advent of Code 2024, day 14, part one
//

#include <algorithm>
#include <array>
#include <cstdio>
#include <functional>
#include <print>
#include <ranges>
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

auto safety_factor(const auto &robots)
{
	std::array<std::array<int, 2>, 2> quadrant_counts = {};

	for (const auto &robot : robots) {
		auto [x, y] = robot.pos;

		if (x == 50 || y == 51) {
			continue;
		}

		++quadrant_counts[y > 51][x > 50];
	}

	return std::ranges::fold_left(quadrant_counts | std::views::join, 1, std::multiplies{});
}

int main()
{
	auto robots = read_robots();

	std::println("read {} robots", robots.size());

	for (int i = 0; i < 100; ++i) {
		step_robots(robots, 101, 103);
	}

	std::println("{}", safety_factor(robots));
}
