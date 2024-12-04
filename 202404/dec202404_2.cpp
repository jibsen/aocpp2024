//
// Advent of Code 2024, day 4, part two
//

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

int count_x_mas(const auto &map, int x, int y)
{
	if (map[y][x] != 'A') {
		return 0;
	}

	auto nw = map[y - 1][x - 1];
	auto ne = map[y - 1][x + 1];
	auto se = map[y + 1][x + 1];
	auto sw = map[y + 1][x - 1];

	return ((nw == 'M' && se == 'S') || (nw == 'S' && se == 'M'))
	    && ((ne == 'M' && sw == 'S') || (ne == 'S' && sw == 'M'));
}

int main()
{
	auto map = read_and_pad_map();

	int total_xmas = 0;

	for (int y = 1; y < std::ssize(map); ++y) {
		for (int x = 1; x < std::ssize(map.front()); ++x) {
			total_xmas += count_x_mas(map, x, y);
		}
	}

	std::println("found {} X-MAS", total_xmas);
}
