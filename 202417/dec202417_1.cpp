//
// Advent of Code 2024, day 17, part one
//

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <cstdlib>
#include <format>
#include <iostream>
#include <iterator>
#include <print>
#include <ranges>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

using Program = std::vector<int>;

auto read_registers() -> std::tuple<std::uint64_t, std::uint64_t, std::uint64_t>
{
	std::string line;

	std::getline(std::cin, line);
	std::uint64_t a = std::stol(line.substr(line.find(':') + 1));

	std::getline(std::cin, line);
	std::uint64_t b = std::stol(line.substr(line.find(':') + 1));

	std::getline(std::cin, line);
	std::uint64_t c = std::stol(line.substr(line.find(':') + 1));

	return {a, b, c};
}

Program read_program()
{
	std::string line;

	std::getline(std::cin, line);

	if (line.empty()) {
		std::getline(std::cin, line);
	}

	line.erase(0, line.find(':') + 2);

	return line | std::views::split(',') | std::views::transform(
		[](const auto part) {
			return std::stoi(std::string(part.data(), part.size()));
		}
	) | std::ranges::to<std::vector>();
}

enum class Opcode { adv, bxl, bst, jnz, bxc, out, bdv, cdv };

struct Debugger {
	explicit Debugger(Program program, std::uint64_t a, std::uint64_t b, std::uint64_t c)
		: program(std::move(program)), registers({a, b, c}) {}

	bool step();
	void run();

	void print_state() const;

	void print_output() const;

private:
	std::uint64_t get_combo(int combo) {
		switch (combo)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			return combo;
		case 4:
			return registers[0];
		case 5:
			return registers[1];
		case 6:
			return registers[2];
		default:
			std::println(std::cerr, "invalid combo operand {}", combo);
			std::exit(1);
		}
	}

	Program program;
	std::array<std::uint64_t, 3> registers = {};
	std::size_t ip = 0;
	std::vector<int> output;
};

void Debugger::print_state() const
{
	std::println("A={} B={} C={} IP={}", registers[0], registers[1], registers[2], ip);
}

void Debugger::print_output() const
{
	std::ranges::copy(output, std::ostream_iterator<int>(std::cout, ","));
	std::cout << '\n';
}

bool Debugger::step()
{
	if (ip + 2 > program.size()) {
		return true;
	}

	auto opcode = Opcode(program[ip++]);

	std::uint64_t operand = program[ip++];

	switch (opcode) {
	case Opcode::adv:
		registers[0] >>= get_combo(operand);
		break;
	case Opcode::bxl:
		registers[1] ^= operand;
		break;
	case Opcode::bst:
		registers[1] = get_combo(operand) & 7U;
		break;
	case Opcode::jnz:
		if (registers[0] != 0) {
			ip = operand;
		}
		break;
	case Opcode::bxc:
		registers[1] ^= registers[2];
		break;
	case Opcode::out:
		output.emplace_back(get_combo(operand) & 7U);
		break;
	case Opcode::bdv:
		registers[1] = registers[0] >> get_combo(operand);
		break;
	case Opcode::cdv:
		registers[2] = registers[0] >> get_combo(operand);
		break;
	default:
		std::println(std::cerr, "invalid opcode {}", std::to_underlying(opcode));
		std::exit(1);
	}

	return false;
}

void Debugger::run()
{
	while (!step()) {
		// nothing
	}
}

int main()
{
	auto [a, b, c] = read_registers();

	std::println("{} {} {}", a, b, c);

	auto program = read_program();

	std::println("program size {}", program.size());

	Debugger debugger(program, a, b ,c);

	debugger.run();

	debugger.print_state();

	debugger.print_output();
}
