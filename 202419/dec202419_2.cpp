//
// Advent of Code 2024, day 19, part two
//

#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

auto read_towels()
{
	using namespace std::literals;

	std::string line;

	std::getline(std::cin, line);

	return line | std::views::split(", "sv) | std::views::transform(
		[](const auto part) {
			return std::string(part.data(), part.size());
		}
	) | std::ranges::to<std::vector>();
}

long ways_to_cover_recursive(const auto &towels, std::string_view design, auto &memory)
{
	if (design.empty()) {
		return 1;
	}

	if (auto it = memory.find(design); it != memory.end()) {
		return it->second;
	}

	long ways = 0;

	for (const auto &towel : towels) {
		if (design.starts_with(towel)) {
			ways += ways_to_cover_recursive(towels, design.substr(towel.size()), memory);
		}
	}

	memory.emplace(design, ways);

	return ways;
}

long ways_to_cover(const auto &towels, std::string_view design)
{
	std::unordered_map<std::string_view, long> memory;

	return ways_to_cover_recursive(towels, design, memory);
}

int main()
{
	auto towels = read_towels();

	std::println("{} towels", towels.size());

	long total_ways_to_cover = 0;

	for (std::string design; std::getline(std::cin, design); ) {
		if (!design.empty()) {
			total_ways_to_cover += ways_to_cover(towels, design);
		}
	}

	std::println("{} ways to cover", total_ways_to_cover);
}
