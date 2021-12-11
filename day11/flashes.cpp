#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <algorithm>

std::tuple<int64_t, int64_t, std::vector<int64_t>> parseInput(std::string inputFileName) {

	std::ifstream inputFile(inputFileName, std::ios::in);
	std::vector<int64_t> octopusEnergy;

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString;
	int64_t row = 0, col = 0;

	inputFile >> unusedString;
	row++;
	for (auto c : unusedString) {
		octopusEnergy.push_back(c - 48);	
		col++;
		
	}

	while (inputFile >> unusedString) {
		row++;
		for (auto c : unusedString) {
			octopusEnergy.push_back(c - 48);	
		}
	}

	return {row, col, octopusEnergy};
}

void chargeEnergy(int64_t octopucLoc, std::vector<int64_t> &octopusEnergy, int64_t maxRow, int64_t maxCol, std::unordered_set<int64_t> &flashLocation) {
	
	int64_t oRow = octopucLoc / maxCol;
	int64_t oCol = octopucLoc % maxCol;
	int64_t rCol = oCol + 1;
	int64_t lCol = oCol - 1;
	int64_t uRow = oRow - 1; 
	int64_t lRow = oRow + 1;

	if (flashLocation.find(octopucLoc) != flashLocation.end()) return;

	if (octopusEnergy[octopucLoc] < 9)
		octopusEnergy[octopucLoc]++;	
	else {
		octopusEnergy[octopucLoc] = 0;
		flashLocation.insert(octopucLoc);

		if (lCol >= 0) 
			chargeEnergy(octopucLoc - 1, octopusEnergy, maxRow, maxCol, flashLocation);
		if (rCol < maxCol) 
			chargeEnergy(octopucLoc + 1, octopusEnergy, maxRow, maxCol, flashLocation); 
		if (uRow >= 0) 
			chargeEnergy(octopucLoc - maxCol, octopusEnergy, maxRow, maxCol, flashLocation);
		if (lRow < maxRow) 
			chargeEnergy(octopucLoc + maxCol, octopusEnergy, maxRow, maxCol, flashLocation);
		if (lCol >= 0 and uRow >=0) 
			chargeEnergy(octopucLoc - maxCol - 1, octopusEnergy, maxRow, maxCol, flashLocation);
		if (rCol < maxCol and uRow >=0)
			chargeEnergy(octopucLoc - maxCol + 1, octopusEnergy, maxRow, maxCol, flashLocation);
		if (lCol >= 0 and lRow < maxRow) 
			chargeEnergy(octopucLoc + maxCol - 1, octopusEnergy, maxRow, maxCol, flashLocation);
		if (rCol < maxCol and lRow < maxRow)
			chargeEnergy(octopucLoc + maxCol + 1, octopusEnergy, maxRow, maxCol, flashLocation);
	}
}

void displayEnergy(std::vector<int64_t> octopusEnergy, int64_t maxCol) {
	uint32_t i = 0;
	std::cout << "-----------------------" << std::endl;
	for (auto oE : octopusEnergy) {
		std::cout << oE << ",";
		i++;
		if (i >=maxCol) {
			std::cout << "\r\n";
			i = 0;
		}
	}
	std::cout << "-----------------------" << std::endl;
}

int64_t countFlashes(std::tuple<int64_t, int64_t, std::vector<int64_t>> octopuses, uint32_t steps) {
	int64_t eRow, eCol;
	std::vector<int64_t> oEnergy;
	std::tie (eRow, eCol, oEnergy) = octopuses;	
	std::unordered_set<int64_t> flashLocation;
	int64_t flashes = 0;

	//displayEnergy(oEnergy, eCol);

	for (auto s = 0; s < steps; s++) {
		for (auto i = 0; i < oEnergy.size(); i++) {
			chargeEnergy(i, oEnergy, eRow, eCol, flashLocation); 
		}
		
		flashes += flashLocation.size();
		flashLocation.clear();
		//displayEnergy(oEnergy, eCol);
	}

	return flashes;
}

int64_t firstSyncFlashes(std::tuple<int64_t, int64_t, std::vector<int64_t>> octopuses) {
	int64_t eRow, eCol;
	std::vector<int64_t> oEnergy;
	std::tie (eRow, eCol, oEnergy) = octopuses;	
	std::unordered_set<int64_t> flashLocation;
	int64_t syncFlashes = 0, step = 1;

	//displayEnergy(oEnergy, eCol);

	while (1) {
		for (auto i = 0; i < oEnergy.size(); i++) {
			chargeEnergy(i, oEnergy, eRow, eCol, flashLocation); 
		}
		
		syncFlashes = flashLocation.size();
		flashLocation.clear();
		if (syncFlashes == oEnergy.size())
			return step;
		step++;
		//displayEnergy(oEnergy, eCol);
	}

	return 0;
}

int main() {

	std::tuple<int64_t, int64_t, std::vector<int64_t>> octopuses = parseInput("input.txt");

	std::cout << "Flashes = " << countFlashes(octopuses, 100) << std::endl;

	std::cout << "First Sync flashes at " << firstSyncFlashes(octopuses) << std::endl;

	return 0;
}

