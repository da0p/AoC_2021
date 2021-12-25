#include <iostream>
#include <fstream>
#include <vector>
#include <set>

typedef enum {
	UNKNOWN,
	EAST_SC,
	SOUTH_SC
}SeaCucumberType;

typedef struct Location {
	SeaCucumberType currentType;
	SeaCucumberType previousType;
} Location;

std::vector<std::vector<Location>> parseInput(std::string inputFileName) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "Could not open file" << std::endl;
		exit(EXIT_FAILURE);
	}

	uint32_t i = 0, j = 0;
	std::string unusedString;
	std::vector<std::vector<Location>> seaCucumbers;
	std::vector<Location> oneRow;

	while (inputFile >> unusedString) {
		for (auto c : unusedString) {
			if (c == '>') {
				oneRow.push_back({EAST_SC, UNKNOWN});
			}
			else if (c == 'v') {
				oneRow.push_back({SOUTH_SC, UNKNOWN});
			}
			else {
				oneRow.push_back({UNKNOWN, UNKNOWN});
			}
			j++;
		}
		seaCucumbers.push_back(oneRow);
		oneRow.clear();
		i++;
		j = 0;
	}

	return seaCucumbers;
}

void moveSeaCucumber(uint32_t row, uint32_t col, std::vector<std::vector<Location>> &all, std::set<std::pair<uint32_t, uint32_t>> &moved)
{
	auto currentRow = row;
	auto currentCol = col;
	auto nextCol = currentCol == all[0].size() - 1 ? 0 : currentCol + 1;
	auto nextRow = currentRow == all.size() - 1 ? 0 : currentRow + 1;
	auto prevRow = currentRow == 0 ? all.size() - 1 : currentRow - 1;
	auto prevCol = currentCol == 0 ? all[0].size() - 1 : currentCol - 1;

	if (moved.find({currentRow, currentCol}) != moved.end()) return;

	if (all[currentRow][currentCol].currentType == UNKNOWN) {
		if (all[currentRow][prevCol].currentType == EAST_SC and
			moved.find({currentRow, prevCol}) == moved.end()) {
			all[currentRow][prevCol].currentType = UNKNOWN;
			all[currentRow][prevCol].previousType = EAST_SC;
			all[currentRow][currentCol].currentType = EAST_SC;
			all[currentRow][currentCol].previousType = UNKNOWN;
			moved.insert({currentRow, currentCol});
			moved.insert({currentRow, prevCol});
			if (all[prevRow][prevCol].currentType == SOUTH_SC and
			    moved.find({prevRow, prevCol}) == moved.end()) {
				all[prevRow][prevCol].currentType = UNKNOWN;
				all[prevRow][prevCol].previousType = SOUTH_SC;
				all[currentRow][prevCol].currentType = SOUTH_SC;
				all[currentRow][prevCol].previousType = UNKNOWN;
				moved.insert({prevRow, prevCol});
			}
			return;
		}

		if (all[prevRow][currentCol].currentType == SOUTH_SC and
			moved.find({prevRow, currentCol}) == moved.end()) {
			all[prevRow][currentCol].currentType = UNKNOWN;
			all[prevRow][currentCol].previousType = SOUTH_SC;
			all[currentRow][currentCol].currentType = SOUTH_SC;
			all[currentRow][currentCol].previousType = UNKNOWN; 
			moved.insert({prevRow, currentCol});
			moved.insert({currentRow, currentCol});
			return;
		}
	}
}

void displaySC(std::vector<std::vector<Location>> seaCucumbers) {

	std::cout << "--------------------------------------" << std::endl;
	for (uint32_t i = 0; i < seaCucumbers.size(); i++) {
		for (uint32_t j = 0; j < seaCucumbers[0].size(); j++) {
			if (seaCucumbers[i][j].currentType == UNKNOWN) std::cout << ".";
			else if (seaCucumbers[i][j].currentType == EAST_SC) std::cout << ">";
			else std::cout << "v";
		}
		std::cout << std::endl;
	}
}

void simulateSC(std::vector<std::vector<Location>> seaCucumbers) {

	uint32_t steps = 0;
	std::set<std::pair<uint32_t, uint32_t>> movedLoc;
	//displaySC(seaCucumbers);
	while (1) {
		for (uint32_t i = 0; i < seaCucumbers.size(); i++) {
			for (uint32_t j = 0; j < seaCucumbers[0].size(); j++) {
				moveSeaCucumber(i, j, seaCucumbers, movedLoc);
			}
		}
		//displaySC(seaCucumbers);
		if (movedLoc.empty()) break;
		movedLoc.clear();

		steps++;
	}

	//moveSeaCucumber(2, 1, seaCucumbers, movedLoc);

	//displaySC(seaCucumbers);

	std::cout << "Steps = " << steps + 1 << std::endl;
}



int main() {

	auto seaCucumbers = parseInput("input.txt");

	simulateSC(seaCucumbers);
	
	return 0;
}
