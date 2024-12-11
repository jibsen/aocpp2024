//
// Advent of Code 2024, day 11, part one
//

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <print>
#include <ranges>
#include <string>
#include <vector>


auto evaluate_stone(long stone, int depth)
{
	if (depth == 0) {
		return 1L;
	}

	const auto as_str = std::to_string(stone);

	long num_stones = 0;

	if (stone == 0) {
		num_stones = evaluate_stone(1, depth - 1);
	}
	else if (as_str.size() % 2 == 0) {
		long left = std::stol(as_str.substr(0, as_str.size() / 2));
		long right = std::stol(as_str.substr(as_str.size() / 2, std::string::npos));

		num_stones = evaluate_stone(left, depth - 1) + evaluate_stone(right, depth - 1);
	}
	else {
		if (stone > std::numeric_limits<long>::max() / 2024) {
			std::println(std::cerr, "overflow");
		}

		num_stones = evaluate_stone(stone * 2024, depth - 1);
	}

	return num_stones;
}


int main()
{
	std::vector<long> stones(std::istream_iterator<long>{std::cin},
	                         std::istream_iterator<long>{});

	long num_stones = std::ranges::fold_left(stones | std::views::transform(
		[](long stone) { return evaluate_stone(stone, 25); }),
		0L,
		std::plus{}
	);

	std::println("number of stones is {}", num_stones);
}
