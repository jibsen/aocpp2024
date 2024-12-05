//
// Advent of Code 2024, day 5, part two
//

#include <array>
#include <iostream>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

auto read_rules()
{
	std::vector<std::pair<int, int>> rules;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		std::istringstream is(line);

		int before = 0;
		int after = 0;
		char ch = ' ';

		if (is >> before >> ch >> after) {
			rules.emplace_back(before, after);
		}
	}

	return rules;
}

bool pages_match_rules(const auto &pages, const auto &rules)
{
	std::array<int, 100> page_index = {};

	page_index.fill(-1);

	for (int i = 0; i < std::ssize(pages); ++i) {
		page_index[pages[i]] = i;
	}

	for (auto [before, after] : rules) {
		if (page_index[before] != -1 && page_index[after] != -1
		 && page_index[before] > page_index[after]) {
			return false;
		}
	}

	return true;
}

void fix_page_order(auto &pages, const auto &rules)
{
	// Assume we can fix the page order by swapping any two pages
	// that break a rule
	for (bool change = true; change; ) {
		change = false;

		std::array<int, 100> page_index = {};

		page_index.fill(-1);

		for (int i = 0; i < std::ssize(pages); ++i) {
			page_index[pages[i]] = i;
		}

		for (auto [before, after] : rules) {
			if (page_index[before] != -1 && page_index[after] != -1
			 && page_index[before] > page_index[after]) {
				std::swap(pages[page_index[before]], pages[page_index[after]]);
				change = true;
				break;
			}
		}
	}
}

int main()
{
	auto rules = read_rules();

	long median_sum = 0;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		auto pages = line | std::views::split(',') | std::views::transform(
			[](const auto part) {
				return std::stoi(std::string(part.data(), part.size()));
			}
		) | std::ranges::to<std::vector>();

		if (!pages_match_rules(pages, rules)) {
			fix_page_order(pages, rules);

			median_sum += pages[pages.size() / 2];
		}
	}

	std::println("median sum is {}", median_sum);
}
