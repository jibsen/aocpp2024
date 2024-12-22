//
// Advent of Code 2024, day 22, part two
//

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

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

int main()
{
	std::vector<std::uint32_t> secrets(std::istream_iterator<std::uint32_t>{std::cin},
	                                   std::istream_iterator<std::uint32_t>{});

	// For each tuple of 4 changes, we store the sum of bananas it will
	// give. To avoid duplicates, we store the initial secret of the last
	// monkey that added to the sum as an id.
	std::unordered_map<
		std::tuple<std::int8_t, std::int8_t, std::int8_t, std::int8_t>,
	        std::pair<std::uint32_t, long>,
		TupleHash
	> bananas;

	for (const auto initial_secret : secrets) {
		std::array<std::int8_t, 4> changes = {};

		auto secret = initial_secret;

		std::int8_t prev_digit = secret % 10;

		for (int round = 0; round < 2000; ++round) {
			secret = secret ^ (secret << 6);
			secret &= (std::uint32_t{1} << 24) - 1;
			secret = secret ^ (secret >> 5);
			secret &= (std::uint32_t{1} << 24) - 1;
			secret = secret ^ (secret << 11);
			secret &= (std::uint32_t{1} << 24) - 1;

			std::int8_t digit = secret % 10;

			std::ranges::rotate(changes, changes.begin() + 1);

			changes.back() = digit - prev_digit;

			if (round >= 3) {
				if (auto [it, success] = bananas.emplace(changes, std::make_pair(initial_secret, digit)); !success) {
					if (it->second.first != initial_secret) {
						it->second.first = initial_secret;
						it->second.second += digit;
					}
				}
			}

			prev_digit = digit;
		}
	}

	long max_bananas = std::ranges::max(bananas | std::views::values | std::views::elements<1>);

	std::println("max bananas {}", max_bananas);
}
