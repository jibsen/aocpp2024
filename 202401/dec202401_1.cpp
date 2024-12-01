//
// Advent of Code 2024, day 1, part one
//

#include <algorithm>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <vector>

int main()
{
	std::vector<long> left_numbers;
	std::vector<long> right_numbers;

	for (long left = 0, right = 0; std::cin >> left >> right; ) {
		left_numbers.push_back(left);
		right_numbers.push_back(right);
	}

	std::ranges::sort(left_numbers);
	std::ranges::sort(right_numbers);

	auto total_distance = std::ranges::fold_left(
		std::views::zip_transform(
			[](auto left, auto right) {
				return std::abs(right - left);
			},
			left_numbers,
			right_numbers

		),
		0L,
		std::plus{}
	);

	std::println("total distance is {}", total_distance);
}
