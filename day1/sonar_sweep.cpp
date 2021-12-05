#include <iostream>
#include <fstream>
#include <iomanip>


int main() {

	uint32_t depth;
	uint32_t checkPoint;
	uint32_t total = 0;

	std::ifstream inputFile("input.txt", std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	// get the first check point
	inputFile >> checkPoint;

	while (inputFile >> depth) {
		if (depth > checkPoint) total++;
		checkPoint = depth;
	}

	std::cout << "Number of increasing points: " << total << std::endl;

	return 0;
}


