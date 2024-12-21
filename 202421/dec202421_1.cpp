//
// Advent of Code 2024, day 21, part one
//

#include <array>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <print>
#include <queue>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

auto read_codes()
{
	std::vector<std::string> codes;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		codes.push_back(std::move(line));
	}

	return codes;
}

constexpr auto taxicab_dist(int x1, int y1, int x2, int y2)
{
	return std::abs(x2 - x1) + std::abs(y2 - y1);
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

			moves.push_back(std::move(move));
		}
	}

	if (moves.empty()) {
		moves.push_back("");
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

			moves.push_back(std::move(move));
		}
	}

	if (moves.empty()) {
		moves.push_back("");
	}

	return moves;
}

auto get_moves_for_code(const auto &code)
{
	std::vector<std::string> moves = {""};

	char from = 'A';

	for (char to : code) {
		std::vector<std::string> new_moves;

		auto next_moves = get_numeric_moves(from, to);

		for (const auto &[move, next_move] : std::views::cartesian_product(moves, next_moves)) {
			new_moves.push_back(move + next_move + "A");
		}

		moves.swap(new_moves);

		from = to;
	}

	return moves;
}

auto get_moves_for_moves(const auto &in_moves)
{
	std::vector<std::string> out_moves;

	for (const auto &in_move : in_moves) {
		std::vector<std::string> moves = {""};

		char from = 'A';

		for (char to : in_move) {
			std::vector<std::string> new_moves;

			auto next_moves = get_directional_moves(from, to);

			for (const auto &[move, next_move] : std::views::cartesian_product(moves, next_moves)) {
				new_moves.push_back(move + next_move + "A");
			}

			moves.swap(new_moves);

			from = to;
		}

		out_moves.insert(out_moves.end(), moves.begin(), moves.end());
	}

	return out_moves;
}

int main()
{
	auto codes = read_codes();

	long complexity_sum = 0;

	for (const auto &code : codes) {
		std::println("---{}---", code);

		auto moves = get_moves_for_moves(get_moves_for_moves(get_moves_for_code(code)));

		auto min_length = std::ranges::min(moves | std::views::transform(
			[](const auto &str) {
				return str.size();
			}
		));

		std::println("{} * {}", min_length, std::stol(code.substr(0, code.size() - 1)));

		complexity_sum += min_length * std::stol(code.substr(0, code.size() - 1));
	}

	std::println("complexity sum {}", complexity_sum);
}
