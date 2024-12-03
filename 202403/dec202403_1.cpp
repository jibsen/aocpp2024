//
// Advent of Code 2024, day 3, part one
//

#include <iostream>
#include <print>
#include <regex>
#include <string>

int main()
{
	std::string memory;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		memory.append(line);
	}

	std::println("memory size is '{}'", memory.size());

	const std::regex mul_re(R"=(mul\((\d{1,3}),(\d{1,3})\))=");

	long total = 0;

	for (std::sregex_iterator it(memory.begin(), memory.end(), mul_re), it_end; it != it_end; ++it) {
		const std::smatch &match = *it;

		total += std::stol(match[1].str()) * std::stol(match[2].str());
	}

	std::println("total is {}", total);
}
