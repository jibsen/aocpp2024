//
// Advent of Code 2024, day 22, part one
//

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <print>
#include <vector>

int main()
{
	std::vector<std::uint32_t> secrets(std::istream_iterator<std::uint32_t>{std::cin},
	                                   std::istream_iterator<std::uint32_t>{});

	for (auto &secret : secrets) {
		for (int round = 0; round < 2000; ++round) {
			secret = secret ^ (secret << 6);
			secret &= (std::uint32_t{1} << 24) - 1;
			secret = secret ^ (secret >> 5);
			secret &= (std::uint32_t{1} << 24) - 1;
			secret = secret ^ (secret << 11);
			secret &= (std::uint32_t{1} << 24) - 1;
		}
	}

	auto sum = std::ranges::fold_left(secrets, 0ULL, std::plus{});

	std::println("sum is {}", sum);
}
