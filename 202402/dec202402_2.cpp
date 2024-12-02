//
// Advent of Code 2024, day 2, part two
//

#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

bool is_valid(const auto &report)
{
	if (!(std::ranges::is_sorted(report) || std::ranges::is_sorted(report, std::greater<>{}))) {
		return false;
	}

	for (auto [a, b] : std::views::adjacent<2>(report)) {
		auto difference = std::abs(b - a);

		if (difference < 1 || difference > 3) {
			return false;
		}
	}

	return true;
}

int main()
{
	long num_valid = 0;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		std::istringstream is(line);

		std::vector<long> report(std::istream_iterator<long>{is},
		                         std::istream_iterator<long>{});

		if (is_valid(report)) {
			++num_valid;
		}
		else {
			for (auto it = report.begin(); it != report.end(); ++it) {
				std::vector<long> report_without_it;

				report_without_it.reserve(report.size());

				report_without_it.assign(report.begin(), it);
				report_without_it.insert(report_without_it.end(), it + 1, report.end());

				if (is_valid(report_without_it)) {
					++num_valid;
					break;
				}
			}
		}
	}

	std::println("{} valid reports", num_valid);
}
