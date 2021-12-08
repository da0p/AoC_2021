#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>

std::vector<uint32_t> parseInput(std::string inputFileName) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString, token;
	std::vector<uint32_t> crabPos;

	inputFile >> unusedString;

	std::istringstream ss(unusedString);
	while(std::getline(ss, token, ',')) {
		crabPos.push_back(stoi(token));
	}

	return crabPos;
}


uint32_t leastFuelPosition(std::vector<uint32_t> crabPos) {

	std::vector<uint32_t> pos;
	uint32_t sum = 0;

	for (auto i = 0; i < crabPos.size(); i++) {
		for (auto j = 0; j < crabPos.size(); j++) {
			if (crabPos[j] > crabPos[i])
				sum += crabPos[j] - crabPos[i];
			else 
				sum += crabPos[i] - crabPos[j];
		}
		pos.push_back(sum);
		sum = 0;
	}


	auto it = std::min_element(std::begin(pos), std::end(pos));

	return *it;
}

uint32_t movingLeaseFuelPosition(std::vector<uint32_t> crabPos) {

	std::vector<uint32_t> pos;
	uint32_t sum = 0;

	auto maxPos = *std::max_element(crabPos.begin(), crabPos.end());
	std::vector<uint32_t> max;

	for (auto i = 0; i < maxPos; i++) {
		max.push_back(i);
		for (auto j = 0; j < crabPos.size(); j++) {
			if (crabPos[j] > i) {
				sum += (crabPos[j] - i) * (crabPos[j] - i + 1) / 2;
			}
			else {
				sum += (i - crabPos[j]) * (i - crabPos[j] + 1) / 2;
			}
		}
		pos.push_back(sum);
		sum = 0;
	}

	auto it = std::min_element(pos.begin(), pos.end());

	return *it;
}

int main() {



	std::vector<uint32_t> crabPos = parseInput("input.txt");

	std::cout << leastFuelPosition(crabPos) << std::endl;

	std::cout << movingLeaseFuelPosition(crabPos) << std::endl;

	return 0;
}
