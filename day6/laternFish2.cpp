#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <numeric>

std::vector<uint64_t>  parseInput(std::string inputFileName, uint64_t tankSize)
{
	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString, token;
	std::vector<uint64_t> laternFishTank(tankSize);

	inputFile >> unusedString;

	std::istringstream ss(unusedString);
	while (std::getline(ss, token, ',')) {
		laternFishTank[stoi(token)]++;
	}

	return laternFishTank;
}

void display(std::vector<uint64_t> laternFishTank) {
	for (auto lF : laternFishTank) {
		std::cout << lF << ", " << std::endl;
	}
}

uint64_t calculatePopulation(std::vector<uint64_t> laternFishTank, uint32_t days) {

	for (auto day = 0; day < days; day++) {
		auto toBeBorn = laternFishTank[0];
		laternFishTank[0] = 0;

		//display(laternFishTank);
		for (auto i = 1; i < laternFishTank.size(); i++) {
			laternFishTank[i - 1] += laternFishTank[i];
			laternFishTank[i] = 0;
		}
		laternFishTank[6] += toBeBorn;
		laternFishTank[8] += toBeBorn;
	}

	uint64_t sum = 0;
	for (auto lF : laternFishTank) 
		sum += lF;

	return sum;
}

int main() {
	
	std::vector<uint64_t> laternFishes = parseInput("input.txt", 9);

	std::cout << laternFishes.size() << std::endl;

	std::cout << calculatePopulation(laternFishes, 256);

	return 0;
}
