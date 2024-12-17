//
// Advent of Code 2024, day 17, part two
//

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <iostream>
#include <limits>
#include <print>
#include <ranges>
#include <string>
#include <vector>

//
// The program 2,4,1,3,7,5,4,7,0,3,1,5,5,5,3,0 contains these instructions:
//
//     bst 4  :  B = A % 8
//     bxl 3  :  B = B ^ 3
//     cdv 5  :  C = A >> B
//     bxc 7  :  B = B ^ C
//     adv 3  :  A = A / 8
//     bxl 5  :  B = B ^ 5
//     out 5  :  OUT B & 7U
//     jnz 0  :  JNZ 0
// 
// We note that each round only depends on the value in A, and that it uses
// the low 3 bits to get an index for a further 3 bits 0-7 positions
// further into A, which are combined to form each output.
//
// So we can work backwards, finding the lowest number that produces the
// last digit of the program and then shifting in new bits that form each
// of the output values.
//

auto read_program()
{
	std::string line;

	while (!line.starts_with("Program:")) {
		std::getline(std::cin, line);
	}

	line.erase(0, line.find(':') + 2);

	return line | std::views::split(',') | std::views::transform(
		[](const auto part) {
			return std::stoi(std::string(part.data(), part.size()));
		}
	) | std::ranges::to<std::vector>();
}

std::uint64_t find_quine_recursive(const auto &program, int idx, std::uint64_t cur)
{
	if (idx == program.size()) {
		return cur;
	}

	std::uint64_t res = std::numeric_limits<std::uint64_t>::max();

	for (std::uint64_t new_bits = 0; new_bits < 8; ++new_bits) {
		std::uint64_t a = (cur << 3) + new_bits;
		std::uint64_t b = a & 7U;
		b ^= 3U;
		std::uint64_t c = a >> b;
		b ^= c;
		// Since we are working in reverse, we are shifting 3 bits
		// into A above rather than shifting them out here
		// a >>= 3;
		b ^= 5U;

		if ((b & 7U) == program[idx]) {
			res = std::min(res, find_quine_recursive(program, idx + 1, a));
		}
	}

	return res;
}

std::uint64_t find_quine(auto program)
{
	std::ranges::reverse(program);

	// The first loop may read 3 bits from A at a position up to 7 bits
	// in, but we are also shifting in 3 new bits, so the range of
	// possible starting values is 0-127
	for (std::uint64_t start_bits = 0; start_bits < 128; ++start_bits) {
		auto val = find_quine_recursive(program, 0, start_bits);

		if (val != std::numeric_limits<std::uint64_t>::max()) {
			return val;
		}
	}

	return 0;
}

auto run(std::uint64_t a, std::uint64_t b, std::uint64_t c)
{
	std::uint64_t out = 0;

	do {
		b = a & 7U;
		b ^= 3U;
		c = a >> b;
		b ^= c;
		a >>= 3;
		b ^= 5U;
		out = out * 10 + (b & 7U);
	} while (a);

	return out;
}

int main()
{
	auto program = read_program();

	std::println("program size {}", program.size());

	auto quine = find_quine(program);

	if (quine != 0) {
		std::println("{} -> {}", quine, run(quine, 0, 0));
	}
}
