#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <algorithm>

std::vector<int> parseInput(std::string inputFileName, int &rowSize, int &colSize) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString;
	std::vector<int> heightMap;
	int r = 0;

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

std::vector<std::pair<int, int>>  findLowPoints(std::vector<int> heightMap, int rowSize, int colSize) {
	int row = 0, col = 0;
	int  leftIndex = -1, rightIndex = -1, upIndex = -1, lowIndex = -1; 
	std::vector<std::pair<int, int>> lowPoints;

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
			lowPoints.push_back(std::make_pair(heightMap[i], i));
		}

		if (upIndex == -1 and leftIndex != -1 and rightIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - 1]
		    and heightMap[i] < heightMap[i + colSize]) {

			lowPoints.push_back(std::make_pair(heightMap[i], i));
		}

		if (upIndex == -1 and rightIndex == -1 and leftIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i - 1] and heightMap[i] < heightMap[i + colSize]) {

			lowPoints.push_back(std::make_pair(heightMap[i], i));

		}
		if (leftIndex == -1 and upIndex != -1 and rightIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - colSize]
		    and heightMap[i] < heightMap[i + colSize]) {

			lowPoints.push_back(std::make_pair(heightMap[i], i));
		}
		if (leftIndex != -1 and upIndex != -1 and rightIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - 1]
		    and heightMap[i] < heightMap[i + colSize] and heightMap[i] < heightMap[i - colSize]) {

			lowPoints.push_back(std::make_pair(heightMap[i], i));
		}
		if (rightIndex == -1 and leftIndex != -1 and upIndex != -1 and lowIndex != -1
		    and heightMap[i] < heightMap[i - 1] and heightMap[i] < heightMap[i + colSize]
		    and heightMap[i] < heightMap[i - colSize]) {

			lowPoints.push_back(std::make_pair(heightMap[i], i));
		}
		if (leftIndex == -1 and lowIndex == -1 and rightIndex != -1 and upIndex != -1 
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - colSize]) {

			lowPoints.push_back(std::make_pair(heightMap[i], i));
		}
		if (lowIndex == -1 and leftIndex != -1 and upIndex != -1 and rightIndex != -1
		    and heightMap[i] < heightMap[i + 1] and heightMap[i] < heightMap[i - 1]
		    and heightMap[i] < heightMap[i - colSize]) {

			lowPoints.push_back(std::make_pair(heightMap[i], i));
		}
		if (lowIndex == -1 and rightIndex == -1 and upIndex != -1 and leftIndex != -1
		    and heightMap[i] < heightMap[i - 1] and heightMap[i] < heightMap[i - colSize]) {

			lowPoints.push_back(std::make_pair(heightMap[i], i));
		}
	}
	return lowPoints;
}

int calculateRiskLevel(std::vector<int> heightMap, int rowSize, int colSize)
{

	std::vector<std::pair<int, int>> lowPoints = findLowPoints(heightMap, rowSize, colSize);
	int riskLevel = 0;
	for (auto lP : lowPoints) {
		riskLevel += lP.first + 1;
	}

	return riskLevel;
}

void findBasinAtOneLowPoint(int lowPointIndex, std::vector<int> heightMap, 
		int heightMapRowSize, int heightMapColSize, 
		std::unordered_set<int> &searchedPoints, std::vector<int> &basinPoints) {

	int row = 0, col = 0;
	int  leftIndex = -1, rightIndex = -1, upIndex = -1, lowIndex = -1; 

	if (heightMap[lowPointIndex] == 9 || searchedPoints.find(lowPointIndex) != searchedPoints.end()) return;	

	row = lowPointIndex / heightMapColSize;
	col = lowPointIndex % heightMapColSize;
	leftIndex = col - 1;
	rightIndex = col + 1;
	upIndex = row - 1;
	lowIndex = row + 1;

	if (rightIndex >= heightMapColSize) rightIndex = -1;

	if (lowIndex >= heightMapRowSize) lowIndex = -1;

	basinPoints.push_back(heightMap[lowPointIndex]);
	searchedPoints.insert(lowPointIndex);
	if (leftIndex != -1 and heightMap[lowPointIndex] < heightMap[lowPointIndex - 1])
		findBasinAtOneLowPoint(lowPointIndex - 1, heightMap, heightMapRowSize, heightMapColSize, searchedPoints, basinPoints);
	if (rightIndex != -1 and heightMap[lowPointIndex] < heightMap[lowPointIndex + 1])
		findBasinAtOneLowPoint(lowPointIndex + 1, heightMap, heightMapRowSize, heightMapColSize, searchedPoints, basinPoints);
	if (upIndex != -1 and heightMap[lowPointIndex] < heightMap[lowPointIndex - heightMapColSize])
		findBasinAtOneLowPoint(lowPointIndex - heightMapColSize, heightMap, heightMapRowSize, heightMapColSize, searchedPoints, basinPoints);
	if (lowIndex != -1 and heightMap[lowPointIndex] < heightMap[lowPointIndex + heightMapColSize])
		findBasinAtOneLowPoint(lowPointIndex + heightMapColSize, heightMap, heightMapRowSize, heightMapColSize, searchedPoints, basinPoints);
}

int calculateBasin(std::vector<int> heightMap, int rowSize, int colSize) {

	std::vector<std::pair<int, int>> lowPoints = findLowPoints(heightMap, rowSize, colSize);
	std::vector<int> basinPoints;
	std::unordered_set<int> searchedPoints;
	std::vector<int> maxThreeBasins;
	int total = 1;
	
	for (auto lP : lowPoints) {
		findBasinAtOneLowPoint(lP.second, heightMap, rowSize, colSize, searchedPoints, basinPoints);
		auto basinSize = basinPoints.size();
		if (maxThreeBasins.size() < 3) {
			maxThreeBasins.push_back(basinSize);
		}
		else {
			auto it = min_element(maxThreeBasins.begin(), maxThreeBasins.end()) - maxThreeBasins.begin();
			if (basinSize > maxThreeBasins[it]) {
				maxThreeBasins[it] = basinSize;
			}
		}
		basinPoints.clear();
		searchedPoints.clear();
	}

	for (auto basinSize : maxThreeBasins) {
		total *= basinSize;
	}

	return total;
}


int main() {

	int colSize, rowSize;
	std::vector<int> heightMap = parseInput("input.txt", rowSize, colSize);

	std::cout << "RowSize = " << rowSize << ", " << "ColSize = " << colSize << std::endl;

	std::cout << "Risk Level = " << calculateRiskLevel(heightMap, rowSize, colSize) << std::endl;

	std::cout << "Basin: " << calculateBasin(heightMap, rowSize, colSize) << std::endl;

	return 0;
}
