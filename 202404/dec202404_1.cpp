//
// Advent of Code 2024, day 4, part one
//

#include <array>
#include <iostream>
#include <print>
#include <string>
#include <vector>

auto read_and_pad_map()
{
	std::vector<std::string> map;

	// Pad map with '.'
	for (std::string line; std::getline(std::cin, line); ) {
		map.push_back("." + line + ".");
	}

	map.insert(map.begin(), std::string(map.front().size(), '.'));
	map.push_back(std::string(map.front().size(), '.'));

	return map;
}

int count_xmas(const auto &map, int x, int y)
{
	constexpr std::array<std::array<int, 2>, 8> directions = {{
		{ 0, 1 }, { 1, 1 }, { 1, 0 }, { 1, -1 },
		{ 0, -1 }, { -1, -1 }, { -1, 0 }, { -1, 1 }
	}};

	if (map[y][x] != 'X') {
		return 0;
	}

	int num_found = 0;

	for (auto [dx, dy] : directions) {
		if (map[y + 1*dy][x + 1*dx] == 'M'
		 && map[y + 2*dy][x + 2*dx] == 'A'
		 && map[y + 3*dy][x + 3*dx] == 'S') {
			++num_found;
		}
	}

	return num_found;
}

int main()
{
	auto map = read_and_pad_map();

	int total_xmas = 0;

	for (int y = 1; y < std::ssize(map); ++y) {
		for (int x = 1; x < std::ssize(map.front()); ++x) {
			total_xmas += count_xmas(map, x, y);
		}
	}

	std::println("found {} XMAS", total_xmas);
}
