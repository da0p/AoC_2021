#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>

std::vector<uint32_t> parseInput(std::string inputFileName, uint32_t &rowSize, uint32_t &colSize) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString;
	std::vector<uint32_t> heightMap;
	uint32_t r = 0;

	while (inputFile >> unusedString) {
		colSize = unusedString.length();
		r++;
		for (auto c : unusedString) {
			heightMap.push_back(c - 48);
		}
	}

	rowSize = r;
	
	return	heightMap; 
}

uint32_t countLowPoints(std::vector<uint32_t> heightMap, uint32_t rowSize, uint32_t colSize) {
	uint32_t riskLevel = 0;
	int row = 0, col = 0;
	int  leftIndex = -1, rightIndex = -1, upIndex = -1, lowIndex = -1; 

	for (auto i = 0; i < heightMap.size(); i++) {
		row = i / colSize;
		col = i % colSize;
		leftIndex = col - 1;
		rightIndex = col + 1;
		upIndex = row - 1;
		lowIndex = row + 1;

		if (rightIndex >= colSize) rightIndex = -1;

		if (lowIndex >= rowSize) lowIndex = -1;

		if (leftIndex == -1 and upIndex == -1 
		    and rightIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i + 1] 
		    and heightMap[i] < heightMap[i + colSize]) {
			riskLevel += heightMap[i] + 1;
		}

		if (upIndex == -1 and leftIndex != -1 and rightIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - 1]
		    and heightMap[i] < heightMap[i + colSize]) {

			riskLevel += heightMap[i] + 1;
		}

		if (upIndex == -1 and rightIndex == -1 and leftIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i - 1] and heightMap[i] < heightMap[i + colSize]) {

			riskLevel += heightMap[i] + 1;

		}
		if (leftIndex == -1 and upIndex != -1 and rightIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - colSize]
		    and heightMap[i] < heightMap[i + colSize]) {

			riskLevel += heightMap[i] + 1;
		}
		if (leftIndex != -1 and upIndex != -1 and rightIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - 1]
		    and heightMap[i] < heightMap[i + colSize] and heightMap[i] < heightMap[i - colSize]) {

			riskLevel += heightMap[i] + 1;
		}
		if (rightIndex == -1 and leftIndex != -1 and upIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i - 1] and heightMap[i] < heightMap[i + colSize]
		    and heightMap[i] < heightMap[i - colSize]) {

			riskLevel += heightMap[i] + 1;
		}
		if (leftIndex == -1 and lowIndex == -1 and rightIndex != -1 and upIndex != -1 
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - colSize]) {

			riskLevel += heightMap[i] + 1;
		}
		if (lowIndex == -1 and leftIndex != -1 and upIndex != -1 and rightIndex != -1
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - 1]
		    and heightMap[i] < heightMap[i - colSize]) {

			riskLevel += heightMap[i] + 1;
		}
		if (lowIndex == -1 and rightIndex == -1 and upIndex != -1 and leftIndex != -1
		    and heightMap[i] < heightMap[i - 1] and heightMap[i] < heightMap[i - colSize]) {

			riskLevel += heightMap[i] + 1;
		}
	}
	return riskLevel;
}

void display(std::vector<uint32_t> heightMap, uint32_t colSize) {

	uint32_t i = 0;

	for (auto height : heightMap) {
		if (i < colSize) {
			std::cout << height << ",";
			i = 0;
		}
		else {
			std::cout << std::endl;
		}
		i++;
	}
}

int main() {

	uint32_t colSize, rowSize;
	std::vector<uint32_t> heightMap = parseInput("input.txt", rowSize, colSize);

	std::cout << "RowSize = " << rowSize << ", " << "ColSize = " << colSize << std::endl;

	std::cout << countLowPoints(heightMap, rowSize, colSize) << std::endl;

	return 0;
}
