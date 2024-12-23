//
// Advent of Code 2024, day 23, part two
//

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <iterator>
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

auto get_triangles(const Graph &adj)
{
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

	return triangles;
}

int main()
{
	auto adj = read_graph();

	std::println("network of {} computers", adj.size());

	// Use the triangles as basis for cliques
	auto triangles = get_triangles(adj);

	std::vector<std::string> maximum_clique;

	for (const auto &[a, b, c] : triangles) {
		std::vector<std::string> clique = {a, b, c};

		for (const auto &[cur, neighbours] : adj) {
			if (std::ranges::contains(clique, cur)) {
				continue;
			}

			bool connected_to_all = std::ranges::all_of(clique,
				[&](const auto &node) {
					return std::ranges::contains(neighbours, node);
				}
			);

			if (connected_to_all) {
				clique.push_back(cur);
			}
		}

		if (clique.size() > maximum_clique.size()) {
			maximum_clique.swap(clique);
		}
	}

	std::ranges::sort(maximum_clique);

	std::ranges::copy(maximum_clique, std::ostream_iterator<std::string>(std::cout, ","));
	std::cout << '\n';
}
