//
// Advent of Code 2024, day 16, part one
//

#include <cstddef>
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

int main()
{
	auto map = read_map();

	std::println("map {}x{}", map.front().size(), map.size());

	// Find start
	auto [start_x, start_y] = [&map]() -> std::pair<int, int> {
		for (const auto &[y, str] : std::views::enumerate(map)) {
			if (auto x = str.find('S'); x != std::string::npos) {
				return {x, y};
			}
		}
		return {-1, -1};
	}();

	std::unordered_map<std::pair<int, int>, long, PairHash> scores;

	std::priority_queue<std::tuple<long, int, int, int, int>,
	                    std::vector<std::tuple<long, int, int, int, int>>,
			    std::greater<>> pqueue;

	auto check_position = [&](int x, int y, int dx, int dy, long score) {
		if (map[y][x] == '#') {
			return;
		}

		if (auto [it, success] = scores.insert({{x, y}, score}); !success) {
			if (score < it->second) {
				it->second = score;
				pqueue.emplace(score, x, y, dx, dy);
			}
		}
		else {
			pqueue.emplace(score, x, y, dx, dy);
		}
	};

	pqueue.emplace(0, start_x, start_y, 1, 0);

	while (!pqueue.empty()) {
		auto [score, x, y, dx, dy] = pqueue.top();
		pqueue.pop();

		if (map[y][x] == 'E') {
			std::println("score {}", score);
			break;
		}

		check_position(x + dx, y + dy, dx, dy, score + 1);
		check_position(x - dy, y + dx, -dy, dx, score + 1001);
		check_position(x + dy, y - dx, dy, -dx, score + 1001);
	}
}
