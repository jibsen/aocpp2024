//
// Advent of Code 2024, day 9, part two
//

#include <iostream>
#include <list>
#include <print>
#include <tuple>

auto read_map()
{
	std::list<std::tuple<int, int, bool>> drive;

	int id = 0;
	bool free_space = false;

	// Read disk map and convert to blocks in drive
	for (char ch = ' '; std::cin >> ch && ch >= '0' && ch <= '9'; ) {
		int digit = ch - '0';

		if (free_space) {
			drive.emplace_back(-1, digit, true);
		}
		else {
			drive.emplace_back(id, digit, false);
			++id;
		}

		free_space = !free_space;
	}

	return drive;
}

void compact_drive(auto &drive)
{
	for (auto cur = drive.rbegin(); cur != drive.rend(); ++cur) {
		auto &[cur_id, cur_size, cur_was_moved] = *cur;

		// Free space is also marked as moved
		if (cur_was_moved) {
			continue;
		}

		// Here cur is the next set of blocks from the right
		// representing a file that hasn't been handled yet

		// Scan from the left for a free area large enough to
		// hold cur, stopping if we get to cur
		for (auto it = drive.begin(), it_end = --cur.base(); it != it_end; ++it) {
			auto &[id, size, was_moved] = *it;

			if (id != -1 || size < cur_size) {
				continue;
			}
			
			// Insert node corresponding to cur before it
			drive.insert(it, {cur_id, cur_size, true});

			// Resize it to free space left
			size -= cur_size;

			// Turn cur into free space
			cur_id = -1;
			cur_was_moved = true;

			break;
		}
	}
}

auto filesystem_checksum(const auto &drive)
{
	unsigned long long checksum = 0;
	unsigned int pos = 0;

	for (auto &[id, size, _] : drive) {
		for (int i = 0; i < size; ++i) {
			if (id != -1) {
				checksum += pos * static_cast<unsigned long long>(id);
			}

			++pos;
		}
	}

	return checksum;
}

int main()
{
	auto drive = read_map();

	compact_drive(drive);

	std::println("checksum is {}", filesystem_checksum(drive));
}
