#include <iostream>
#include <fstream>
#include <iomanip>

int main() {

	uint32_t fPoint, mPoint, lPoint, bPoint;
	uint32_t windowSum = 0;
	uint32_t checkPoint;
	uint32_t total = 0;

	std::ifstream inputFile("input.txt", std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	// get the first check point
	inputFile >> fPoint >> mPoint >> lPoint;
	windowSum = fPoint + mPoint + lPoint;
	checkPoint = windowSum;

	while (inputFile >> bPoint) {
		fPoint = mPoint;
		mPoint = lPoint;
		lPoint = bPoint;
		windowSum = fPoint + mPoint + lPoint;
		if (windowSum > checkPoint) total++;
		checkPoint = windowSum;
	}

	std::cout << "Number of increasing points: " << total << std::endl;

	return 0;
}


