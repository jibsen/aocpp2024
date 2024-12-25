//
// Advent of Code 2024, day 24, part one
//

#include <cstdint>
#include <iostream>
#include <optional>
#include <print>
#include <ranges>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

auto split(std::string_view sv, std::string_view delim)
{
	return sv | std::views::split(delim) | std::views::transform(
		[](const auto part) {
			return std::string_view(part);
		}
	) | std::ranges::to<std::vector>();
}

enum class Operation {
	And, Or, Xor
};

struct Gate {
	std::string lhs;
	std::string rhs;
	Operation op = Operation::And;
	std::string out;
	std::optional<unsigned int> lhs_val;
	std::optional<unsigned int> rhs_val;
	std::optional<unsigned int> out_val;
};

auto read_inputs()
{
	std::unordered_map<std::string, unsigned int> inputs;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		std::string name = line.substr(0, line.find(':'));
		inputs.emplace(std::move(name), line.back() - '0');
	}

	return inputs;
}

auto read_gates()
{
	std::vector<Gate> gates;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		auto parts = split(line, " ");

		std::string lhs(parts[0]);
		std::string rhs(parts[2]);
		std::string out(parts[4]);

		char op_start = parts[1].front();

		Operation op = op_start == 'A' ? Operation::And
		             : op_start == 'O' ? Operation::Or
			     : Operation::Xor;

		gates.emplace_back(std::move(lhs), std::move(rhs), op, std::move(out));
	}

	return gates;
}

bool update_gates_and_input(auto &gates, auto &inputs)
{
	bool progress = false;

	for (auto &gate : gates) {
		if (gate.out_val) {
			continue;
		}

		if (!gate.lhs_val) {
			if (auto lhs = inputs.find(gate.lhs); lhs != inputs.end()) {
				gate.lhs_val = lhs->second;
			}
		}

		if (!gate.rhs_val) {
			if (auto rhs = inputs.find(gate.rhs); rhs != inputs.end()) {
				gate.rhs_val = rhs->second;
			}
		}

		if (gate.lhs_val && gate.rhs_val) {
			switch (gate.op)
			{
			case Operation::And:
				gate.out_val = gate.lhs_val.value() & gate.rhs_val.value();
				break;
			case Operation::Or:
				gate.out_val = gate.lhs_val.value() | gate.rhs_val.value();
				break;
			case Operation::Xor:
				gate.out_val = gate.lhs_val.value() ^ gate.rhs_val.value();
				break;
			default:
				break;
			}

			inputs.emplace(gate.out, gate.out_val.value());

			progress = true;
		}
	}

	return progress;
}

int main()
{
	auto inputs = read_inputs();

	std::println("{} inputs", inputs.size());

	auto gates = read_gates();

	std::println("{} gates", gates.size());

	for (;;) {
		if (!update_gates_and_input(gates, inputs)) {
			break;
		}
	}

	std::uint64_t result = 0;

	for (const auto &[name, value] : inputs) {
		if (name.starts_with('z') && value != 0) {
			auto index = std::stol(name.substr(1));
			result |= std::uint64_t{1} << index;
		}
	}

	std::println("output: {}", result);
}
