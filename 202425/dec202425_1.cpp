//
// Advent of Code 2024, day 25, part one
//

#include <algorithm>
#include <array>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

auto read_locks_and_keys()
{
	std::vector<std::tuple<int, int, int, int, int>> locks;
	std::vector<std::tuple<int, int, int, int, int>> keys;

	for (std::string first_line; std::getline(std::cin, first_line); ) {
		std::array<int, 5> pins = {};

		for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
			for (auto [idx, ch] : std::views::enumerate(line)) {
				pins[idx] += (ch == '#');
			}
		}

		if (first_line.starts_with('#')) {
			locks.emplace_back(pins);
		}
		else {
			// Correct for last line
			for (auto &pin : pins) {
				--pin;
			}

			keys.emplace_back(pins);
		}

	}

	return std::make_pair(locks, keys);
}

int main()
{
	auto [locks, keys] = read_locks_and_keys();

	std::println("{} locks, {} keys", locks.size(), keys.size());

	auto matches = [](const auto &key, const auto &lock) {
		return std::get<0>(key) <= 5 - std::get<0>(lock)
		    && std::get<1>(key) <= 5 - std::get<1>(lock)
		    && std::get<2>(key) <= 5 - std::get<2>(lock)
		    && std::get<3>(key) <= 5 - std::get<3>(lock)
		    && std::get<4>(key) <= 5 - std::get<4>(lock);
	};

	int num_matching = 0;

	for (const auto &[key, lock] : std::views::cartesian_product(keys, locks)) {
		num_matching += matches(key, lock);
	}

	std::println("{} matching keys", num_matching);
}
