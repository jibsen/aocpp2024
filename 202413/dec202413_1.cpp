//
// Advent of Code 2024, day 13, part one
//

#include <cstdio>
#include <print>

auto lowest_token_cost(int ax, int ay, int bx, int by, int prize_x, int prize_y)
{
	// Solving system of two equations with two unknowns by substitution
	// we get num_a = (bx * prize_y - by * prize_x) / (bx * ay - by * ax),
	// and num_b = (prize_x - ax * num_a) / bx

	auto numerator = bx * prize_y - by * prize_x;
	auto denominator = bx * ay - by * ax;

	if (numerator % denominator == 0) {
		auto num_a = numerator / denominator;

		if ((prize_x - ax * num_a) % bx == 0) {
			auto num_b = (prize_x - ax * num_a) / bx;

			return 3 * num_a + num_b;
		}
	}

	return 0;
}

int main()
{
	int tokens_for_all_prizes = 0;

	for (;;) {
		int ax = 0;
		int ay = 0;
		int bx = 0;
		int by = 0;
		int prize_x = 0;
		int prize_y = 0;

		if (std::scanf("Button A: X+%d, Y+%d\n"
		               "Button B: X+%d, Y+%d\n"
		               "Prize: X=%d, Y=%d\n", &ax, &ay, &bx, &by, &prize_x, &prize_y) != 6) {
			break;
		}

		tokens_for_all_prizes += lowest_token_cost(ax, ay, bx, by, prize_x, prize_y);
	}

	std::println("{}", tokens_for_all_prizes);
}
