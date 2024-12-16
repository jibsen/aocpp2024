//
// Advent of Code 2024, day 16, part two
//

#include <algorithm>
#include <cstddef>
#include <cstdlib>
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

struct TupleHash {
	template<typename... Ts>
	std::size_t operator()(const std::tuple<Ts...> &tuple) const noexcept
	{
		std::size_t hash = 17;
		std::apply([&hash](const auto &... args) {
			(..., (hash = hash * 37 + std::hash<Ts>()(args)));
		}, tuple);
		return hash;
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

bool mark_path_recursive(auto &map, int x, int y, int dx, int dy, long score,
                         long target_score, int end_x, int end_y)
{
	static std::unordered_map<std::tuple<int, int, int, int>, long, TupleHash> seen;

	if (map[y][x] == 'E') {
		return score == target_score;
	}

	if (map[y][x] == '#') {
		return false;
	}

	long min_distance = std::abs(end_x - x) + std::abs(end_y - y) + 1000 * (end_x != x && end_y != y);

	// Stop if shortest distance to end would exceed target score
	if (score + min_distance > target_score) {
		return false;
	}

	// Stop if we have already seen a better path
	if (auto [it, success] = seen.insert({{x, y, dx, dy}, score}); !success) {
		if (score > it->second) {
			return false;
		}

		it->second = score;
	}

	auto p1 = mark_path_recursive(map, x + dx, y + dy, dx, dy, score + 1, target_score, end_x, end_y);
	auto p2 = mark_path_recursive(map, x - dy, y + dx, -dy, dx, score + 1001, target_score, end_x, end_y);
	auto p3 = mark_path_recursive(map, x + dy, y - dx, dy, -dx, score + 1001, target_score, end_x, end_y);

	if (p1 || p2 || p3) {
		map[y][x] = 'O';
		return true;
	}

	return false;
}

void mark_paths_with_score(auto &map, int start_x, int start_y, long target_score)
{
	// Find end
	auto [end_x, end_y] = [&map]() -> std::pair<int, int> {
		for (const auto &[y, str] : std::views::enumerate(map)) {
			if (auto x = str.find('E'); x != std::string::npos) {
				return {x, y};
			}
		}
		return {-1, -1};
	}();

	mark_path_recursive(map, start_x, start_y, 1, 0, 0, target_score, end_x, end_y);
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
			mark_paths_with_score(map, start_x, start_y, score);
			break;
		}

		check_position(x + dx, y + dy, dx, dy, score + 1);
		check_position(x - dy, y + dx, -dy, dx, score + 1001);
		check_position(x + dy, y - dx, dy, -dx, score + 1001);
	}

	std::println("tiles on best paths {}", std::ranges::count(map | std::views::join, 'O') + 1);
}
