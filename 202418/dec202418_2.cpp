//
// Advent of Code 2024, day 18, part two
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

auto read_incoming_bytes()
{
	std::vector<std::pair<int, int>> bytes;

	char ch = ' ';

	for (int x = -1, y = -1; std::cin >> x >> ch >> y && ch == ','; ) {
		bytes.emplace_back(x, y);
	}

	return bytes;
}

auto get_map(int dim)
{
	std::vector<std::string> map(dim + 2, std::string(dim + 2, '.'));

	for (auto &ch : map.front()) {
		ch = '#';
	}

	for (auto &line : map) {
		line.front() = '#';
		line.back() = '#';
	}

	for (auto &ch : map.back()) {
		ch = '#';
	}

	return map;
}

void print_map(const auto &map)
{
	std::ranges::copy(map, std::ostream_iterator<std::string>(std::cout, "\n"));
}

bool has_path(const auto &map)
{
	constexpr std::array<std::array<int, 2>, 4> directions = {{
		{ 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 }
	}};

	std::unordered_set<std::pair<int, int>, PairHash> seen;

	std::queue<std::tuple<int, int, int>> queue;

	queue.emplace(1, 1, 0);

	while (!queue.empty()) {
		auto [x, y, dist] = queue.front();
		queue.pop();

		if (x + 2 == map.front().size() && y + 2 == map.size()) {
			return true;
		}

		if (auto [it, success] = seen.insert({x, y}); !success) {
			continue;
		}

		for (auto [dx, dy] : directions) {
			if (map[y + dy][x + dx] == '.') {
				queue.emplace(x + dx, y + dy, dist + 1);
			}
		}
	}

	return false;
}

int main()
{
	auto bytes = read_incoming_bytes();

	std::println("{} bytes", bytes.size());

	auto map = get_map(71);

	for (auto [x, y] : bytes) {
		map[y + 1][x + 1] = '#';
	}

	for (auto [x, y] : bytes | std::views::reverse) {
		map[y + 1][x + 1] = '.';

		if (has_path(map)) {
			std::println("{},{}", x, y);
			break;
		}
	}
}
