//
// Advent of Code 2024, day 1, part two
//

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <print>
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

	long similarity_score = 0;

	for (std::size_t i = 0, j = 0; i < left_numbers.size(); ++i) {
		long left_num_equal = 1;

		// Count number of occurences in left list and point i at last
		while (i + 1 < left_numbers.size() && left_numbers[i + 1] == left_numbers[i]) {
			++left_num_equal;
			++i;
		}

		// Skip smaller numbers in right list
		while (j < right_numbers.size() && right_numbers[j] < left_numbers[i]) {
			++j;
		}

		long right_num_equal = 0;

		// Count number of occurences in right list
		while (j < right_numbers.size() && right_numbers[j] == left_numbers[i]) {
			++right_num_equal;
			++j;
		}

		similarity_score += left_num_equal * right_num_equal * left_numbers[i];
	}

	std::println("similarity score is {}", similarity_score);
}
