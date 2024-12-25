//
// Advent of Code 2024, day 24, part two
//

#include <algorithm>
#include <array>
#include <bitset>
#include <cstdint>
#include <format>
#include <iostream>
#include <iterator>
#include <optional>
#include <print>
#include <random>
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

auto get_inputs(std::uint64_t x, std::uint64_t y)
{
	std::unordered_map<std::string, int> inputs;

	std::bitset<45> x_bits(x);
	std::bitset<45> y_bits(y);

	for (int i = 0; i < 45; ++i) {
		auto x_name = std::format("x{:02}", i);
		inputs[x_name] = x_bits[i];

		auto y_name = std::format("y{:02}", i);
		inputs[y_name] = y_bits[i];
	}

	return inputs;
}

std::uint64_t simulate_gates(auto gates, std::uint64_t x, std::uint64_t y)
{
	auto inputs = get_inputs(x, y);

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

	return result;
}

auto get_possible_swaps(auto gates)
{
	std::vector<std::pair<int, int>> swaps;

	for (std::uint64_t val = 1; val < std::uint64_t{1} << 45; val <<= 1) {
		auto result = simulate_gates(gates, val, val);

		// If we have a mismatch, try swapping all pairs of output
		// wires and store any that fix the bug
		if (result != val + val) {
			for (int i = 0; i < std::ssize(gates); ++i) {
				for (int j = i + 1; j < std::ssize(gates); ++j) {
					std::swap(gates[i].out, gates[j].out);

					auto new_result = simulate_gates(gates, val, val);

					if (new_result == val + val) {
						swaps.emplace_back(i, j);
					}

					std::swap(gates[i].out, gates[j].out);
				}
			}
		}
	}

	return swaps;
}

auto check_gates(const auto &gates)
{
	for (int i = 0; i < 1000; ++i) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<std::uint64_t> dist;

		auto x = dist(gen) & ((std::uint64_t{1} << 45) - 1);
		auto y = dist(gen) & ((std::uint64_t{1} << 45) - 1);

		auto result = simulate_gates(gates, x, y);

		if (result != x + y) {
			return false;
		}
	}

	return true;
}

auto check_swaps(auto &gates, auto a, auto b, auto c, auto d)
{
	std::swap(gates[a.first].out, gates[a.second].out);
	std::swap(gates[b.first].out, gates[b.second].out);
	std::swap(gates[c.first].out, gates[c.second].out);
	std::swap(gates[d.first].out, gates[d.second].out);

	if (check_gates(gates)) {
		std::array<std::string, 8> result = {
			gates[a.first].out, gates[a.second].out,
			gates[b.first].out, gates[b.second].out,
			gates[c.first].out, gates[c.second].out,
			gates[d.first].out, gates[d.second].out
		};

		std::ranges::sort(result);

		std::ranges::copy(result, std::ostream_iterator<std::string>(std::cout, ","));
		std::cout << '\n';
	}

	std::swap(gates[d.first].out, gates[d.second].out);
	std::swap(gates[c.first].out, gates[c.second].out);
	std::swap(gates[b.first].out, gates[b.second].out);
	std::swap(gates[a.first].out, gates[a.second].out);
}

int main()
{
	// Skip input values
	for (std::string line; std::getline(std::cin, line); ) {
		if (line.empty()) {
			break;
		}
	}

	auto gates = read_gates();

	std::println("{} gates", gates.size());

	auto swaps = get_possible_swaps(gates);

	// Sort order of pairs in wires
	for (auto &[a, b] : swaps) {
		if (a > b) {
			std::swap(a, b);
		}
	}

	// Remove duplicates
	std::ranges::sort(swaps);

	auto res = std::ranges::unique(swaps);

	swaps.erase(res.begin(), res.end());

	std::println("{} possible swaps found", swaps.size());

	// Try all combinations of 4 unique swaps
	for (int i = 0; i < std::ssize(swaps); ++i) {
		for (int j = i + 1; j < std::ssize(swaps); ++j) {
			for (int k = j + 1; k < std::ssize(swaps); ++k) {
				for (int l = k + 1; l < std::ssize(swaps); ++l) {
					check_swaps(gates, swaps[i], swaps[j], swaps[k], swaps[l]);
				}
			}
		}
	}
}
