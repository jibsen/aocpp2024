//
// Advent of Code 2024, day 3, part two
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

	// Replace memory between don't() and do() with spaces
	for (std::string::size_type start = 0; ; ) {
		start = memory.find("don't()", start);

		if (start == std::string::npos) {
			break;
		}

		auto end = memory.find("do()", start);

		if (end == std::string::npos) {
			end = memory.size();
		}

		memory.replace(start, end - start, end - start, ' ');

		start = end;
	}

	const std::regex mul_re(R"=(mul\((\d{1,3}),(\d{1,3})\))=");

	long total = 0;

	for (std::sregex_iterator it(memory.begin(), memory.end(), mul_re), it_end; it != it_end; ++it) {
		const std::smatch &match = *it;

		total += std::stol(match[1].str()) * std::stol(match[2].str());
	}

	std::println("total is {}", total);
}
