//
// Advent of Code 2024, day 23, part one
//

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <print>
#include <ranges>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct TupleHash {
	template<typename... Ts>
	std::size_t operator()(const std::tuple<Ts...> &tuple) const noexcept
	{
		std::size_t hash = 17;
		std::apply([&hash](const auto &... args) {
			(..., (hash = hash * 37 + std::hash<Ts>()(args)));
		}, tuple);
		return hash;
	}
};

using Graph = std::unordered_map<std::string, std::vector<std::string>>;

auto read_graph()
{
	Graph adj;

	for (std::string line; std::getline(std::cin, line) && !line.empty(); ) {
		std::string lhs = line.substr(0, 2);
		std::string rhs = line.substr(3);

		adj[lhs].push_back(rhs);
		adj[rhs].push_back(lhs);
	}

	return adj;
}

int main()
{
	auto adj = read_graph();

	std::println("network of {} computers", adj.size());

	std::unordered_set<std::tuple<std::string, std::string, std::string>, TupleHash> triangles;

	// Brute force all chains of 3 computers, storing
	// them in a set to remove duplicates
	for (const auto &first : adj | std::views::keys) {
		for (const auto &second : adj.at(first)) {
			for (const auto &third : adj.at(second)) {
				if (std::ranges::contains(adj.at(third), first)) {
					std::array<std::string, 3> cycle = { first, second, third };
					std::ranges::sort(cycle);
					triangles.insert(cycle);
				}
			}
		}
	}

	auto one_starts_with_t = [](const auto &elem) {
		const auto &[a, b, c] = elem;
		return a.starts_with('t') || b.starts_with('t') || c.starts_with('t');
	};

	std::println("{} triangles with t", std::ranges::count_if(triangles, one_starts_with_t));
}
