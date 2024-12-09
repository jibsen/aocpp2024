//
// Advent of Code 2024, day 9, part one
//

#include <iostream>
#include <print>
#include <ranges>
#include <utility>
#include <vector>

auto read_map()
{
	std::vector<int> drive;

	int id = 0;
	bool free_space = false;

	// Read disk map and convert to blocks in drive
	for (char ch = ' '; std::cin >> ch && ch >= '0' && ch <= '9'; ) {
		int digit = ch - '0';

		drive.insert(drive.end(), digit, free_space ? -1 : id);

		id += !free_space;

		free_space = !free_space;
	}

	return drive;
}

void compact_drive(auto &drive)
{
	// Move blocks from end to free spaces
	for (long left = 0, right = std::ssize(drive) - 1; left < right; ) {
		// Find first free space from left
		if (drive[left] != -1) {
			++left;
			continue;
		}

		// Find first non-free block from right
		while (right > left && drive[right] == -1) {
			--right;
		}

		// Move the block, if found
		if (right > left) {
			drive[left] = std::exchange(drive[right], -1);
			++left;
			--right;
		}
	}
}

auto filesystem_checksum(const auto &drive)
{
	unsigned long long checksum = 0;

	for (const auto &[pos, id] : std::views::enumerate(drive)) {
		if (id == -1) {
			break;
		}

		checksum += pos * static_cast<unsigned long long>(id);
	}

	return checksum;
}

int main()
{
	auto drive = read_map();

	compact_drive(drive);

	std::println("checksum is {}", filesystem_checksum(drive));
}
