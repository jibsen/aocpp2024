//
// Advent of Code 2024, day 19, part one
//

#include <algorithm>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
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

bool can_cover(const auto &towels, std::string_view design)
{
	if (design.empty()) {
		return true;
	}

	return std::ranges::any_of(towels,
		[&](const auto &towel) {
			return design.starts_with(towel)
			    && can_cover(towels, design.substr(towel.size()));
		}
	);
}

int main()
{
	auto towels = read_towels();

	std::println("{} towels", towels.size());

	int num_can_be_covered = 0;

	for (std::string design; std::getline(std::cin, design); ) {
		if (!design.empty() && can_cover(towels, design)) {
			++num_can_be_covered;
		}
	}

	std::println("{} designs are possible", num_can_be_covered);
}
