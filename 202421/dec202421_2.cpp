//
// Advent of Code 2024, day 21, part two
//

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

struct PairHash {
	template<typename T1, typename T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const noexcept
	{
		std::size_t h1 = std::hash<T1>()(p.first);
		std::size_t h2 = std::hash<T2>()(p.second);
		return (17 * 37 + h1) * 37 + h2;
	}
};

auto read_codes()
{
	std::vector<std::string> codes;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		codes.push_back(std::move(line));
	}

	return codes;
}

auto get_numeric_moves(char from, char to)
{
	static const std::unordered_map<char, std::pair<int, int>> get_coords = {
		{'7', {0, 0}}, {'8', {1, 0}}, {'9', {2, 0}},
		{'4', {0, 1}}, {'5', {1, 1}}, {'6', {2, 1}},
		{'1', {0, 2}}, {'2', {1, 2}}, {'3', {2, 2}},
		{'\0', {0, 3}}, {'0', {1, 3}}, {'A', {2, 3}}
	};

	auto [x1, y1] = get_coords.at(from);
	auto [x2, y2] = get_coords.at(to);

	std::vector<std::string> moves;

	// Try moving horizontally first
	if (x2 - x1 != 0) {
		std::string move;

		auto x = x1;
		auto y = y1;

		// Move right
		for (; x < x2; ++x){
			move.push_back('>');
		}

		// Move left
		for (; x > x2; --x){
			move.push_back('<');
		}

		if (x != 0 || y != 3) {
			// Move up
			for (; y > y2; --y){
				move.push_back('^');
			}

			// Move down
			for (; y < y2; ++y){
				move.push_back('v');
			}

			move.push_back('A');

			moves.push_back(std::move(move));
		}
	}

	// Try moving vertically first
	if (y2 - y1 != 0) {
		std::string move;

		auto x = x1;
		auto y = y1;

		// Move up
		for (; y > y2; --y){
			move.push_back('^');
		}

		// Move down
		for (; y < y2; ++y){
			move.push_back('v');
		}

		if (x != 0 || y != 3) {
			// Move right
			for (; x < x2; ++x){
				move.push_back('>');
			}

			// Move left
			for (; x > x2; --x){
				move.push_back('<');
			}

			move.push_back('A');

			moves.push_back(std::move(move));
		}
	}

	if (moves.empty()) {
		moves.push_back("A");
	}

	return moves;
}

auto get_directional_moves(char from, char to)
{
	static const std::unordered_map<char, std::pair<int, int>> get_coords = {
		{'\0', {0, 0}}, {'^', {1, 0}}, {'A', {2, 0}},
		{'<', {0, 1}}, {'v', {1, 1}}, {'>', {2, 1}}
	};

	auto [x1, y1] = get_coords.at(from);
	auto [x2, y2] = get_coords.at(to);

	std::vector<std::string> moves;

	// Try moving horizontally first
	if (x2 - x1 != 0) {
		std::string move;

		auto x = x1;
		auto y = y1;

		// Move right
		for (; x < x2; ++x){
			move.push_back('>');
		}

		// Move left
		for (; x > x2; --x){
			move.push_back('<');
		}

		if (x != 0 || y != 0) {
			// Move up
			for (; y > y2; --y){
				move.push_back('^');
			}

			// Move down
			for (; y < y2; ++y){
				move.push_back('v');
			}

			move.push_back('A');

			moves.push_back(std::move(move));
		}
	}

	// Try moving vertically first
	if (y2 - y1 != 0) {
		std::string move;

		auto x = x1;
		auto y = y1;

		// Move up
		for (; y > y2; --y){
			move.push_back('^');
		}

		// Move down
		for (; y < y2; ++y){
			move.push_back('v');
		}

		if (x != 0 || y != 0) {
			// Move right
			for (; x < x2; ++x){
				move.push_back('>');
			}

			// Move left
			for (; x > x2; --x){
				move.push_back('<');
			}

			move.push_back('A');

			moves.push_back(std::move(move));
		}
	}

	if (moves.empty()) {
		moves.push_back("A");
	}

	return moves;
}

auto get_move_length_recursive(const std::string &in_move, int depth) -> long long
{
	static std::unordered_map<std::pair<std::string, int>, long long, PairHash> memory;

	if (depth == 0) {
		return std::ssize(in_move);
	}

	if (auto it = memory.find({in_move, depth}); it != memory.end()) {
		return it->second;
	}

	long long length = 0;

	char from = 'A';

	for (auto to : in_move) {
		auto moves = get_directional_moves(from, to);

		auto min_cost = std::ranges::min(moves | std::views::transform(
			[&](const auto &move) {
				return get_move_length_recursive(move, depth - 1);
			}
		));

		length += min_cost;

		from = to;
	}

	memory[{in_move, depth}] = length;

	return length;
}

auto get_move_length(const std::string &move)
{
	return get_move_length_recursive(move, 25);
}

int main()
{
	auto codes = read_codes();

	long long complexity_sum = 0;

	for (const auto &code : codes) {
		std::println("---{}---", code);

		long long length = 0;

		char from = 'A';

		for (auto to : code) {
			auto moves = get_numeric_moves(from, to);

			auto min_cost = std::ranges::min(moves | std::views::transform(
				[&](const auto &move) {
					return get_move_length(move);
				}
			));

			length += min_cost;

			from = to;
		}

		std::println("{} * {}", length, std::stol(code.substr(0, code.size() - 1)));

		complexity_sum += length * std::stol(code.substr(0, code.size() - 1));
	}

	std::println("complexity sum {}", complexity_sum);
}
