//
// Advent of Code 2024, day 7, part two
//

#include <algorithm>
#include <array>
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

constexpr std::uint64_t concat(std::uint64_t a, std::uint64_t b)
{
	std::uint64_t factor = 10;

	while (factor <= b) {
		factor *= 10;
	}

	if (a > std::numeric_limits<std::uint64_t>::max() / factor) {
		return 0;
	}

	return a * factor + b;
}

static_assert(concat(123, 456) == 123456);
static_assert(concat(0, 456) == 456);
static_assert(concat(456, 0) == 4560);

struct Operators {
	void next() {
		for (auto &val : state) {
			++val;

			if (val != 3) {
				break;
			}

			val = 0;
		}
	}

	std::array<std::uint8_t, 64> state = {};
};

std::uint64_t evaluate(const auto &equation, const Operators &ops, std::uint64_t target)
{
	std::uint64_t result = equation.second.front();

	for (const auto &[num, value] : std::views::enumerate(equation.second) | std::views::drop(1)) {
		if (ops.state[num - 1] == 0) {
			if (result > std::numeric_limits<std::uint64_t>::max() - value) {
				return 0;
			}

			result += value;
		}
		else if (ops.state[num - 1] == 1) {
			if (result > std::numeric_limits<std::uint64_t>::max() / value) {
				return 0;
			}

			result *= value;
		}
		else {
			result = concat(result, value);

			if (result == 0) {
				return 0;
			}
		}

		if (result > target) {
			return 0;
		}
	}

	return result;
}

int main()
{
	auto equations = read_equations();

	std::println("read {} equations", equations.size());

	auto can_be_true = [](const auto &equation) {
		for (Operators ops; ops.state[equation.second.size()] == 0; ops.next()) {
			if (evaluate(equation, ops, equation.first) == equation.first) {
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
