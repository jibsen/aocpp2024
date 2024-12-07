//
// Advent of Code 2024, day 7, part one
//

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

auto read_equations()
{
	std::vector<std::pair<std::uint64_t, std::vector<std::uint32_t>>> equations;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		std::istringstream is(line);

		std::uint64_t test_value = 0;

		is >> test_value;

		is.ignore();

		std::vector<std::uint32_t> numbers(std::istream_iterator<std::uint32_t>{is},
		                                   std::istream_iterator<std::uint32_t>{});

		equations.emplace_back(test_value, numbers);
	}

	return equations;
}

std::uint64_t evaluate(const auto &equation, std::uint64_t bits)
{
	std::uint64_t result = equation.second.front();

	for (const auto &[num, value] : std::views::enumerate(equation.second) | std::views::drop(1)) {
		if (bits & (std::uint64_t{1} << (num - 1))) {
			if (result > std::numeric_limits<std::uint64_t>::max() / value) {
				return 0;
			}

			result *= value;
		}
		else {
			if (result > std::numeric_limits<std::uint64_t>::max() - value) {
				return 0;
			}

			result += value;
		}
	}

	return result;
}

int main()
{
	auto equations = read_equations();

	std::println("read {} equations", equations.size());

	auto can_be_true = [](const auto &equation) {
		const std::uint64_t limit = std::uint64_t{1} << equation.second.size();

		for (std::uint64_t bits = 0; bits < limit; ++bits) {
			if (evaluate(equation, bits) == equation.first) {
				return true;
			}
		}

		return false;
	};

	auto calibration_result = std::ranges::fold_left(
		equations | std::views::filter(can_be_true) | std::views::elements<0>,
		std::uint64_t{0},
		std::plus{}
	);

	std::println("total calibration result is {}", calibration_result);
}
