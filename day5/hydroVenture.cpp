#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

void display(std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> location) {
	for (auto it = location.begin(); it != location.end(); it++) {
		for (auto e : it->second) {
			std::cout << "(" << it->first.first
				<< "," << it->first.second
				<< ")  -> "
				<< "(" << e.first
				<< "," << e.second
				<< ")" << std::endl;
		}
	}
}

std::map<std::pair<int, int>, std::vector<std::pair<int, int>>>
parseInput(std::string inputFileName, uint32_t &fieldSize) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString;
	std::string token;
	std::vector<int> coordinates;
	std::vector<std::pair<int, int>> lines;
	std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> maps;
	std::vector<std::pair<int, int>> lineGroup;
	uint32_t fSize = 0;
	int coordinate;

	while (inputFile >> unusedString) {
		if (unusedString.compare("->") != 0) {
			std::istringstream ss(unusedString);
			while(std::getline(ss, token, ',')) {
				coordinate = stoi(token);
				if (coordinate > fSize) {
					fSize = coordinate;
				}
				coordinates.push_back(coordinate);
			}
			lines.push_back(std::make_pair(coordinates[0], coordinates[1]));
			coordinates.clear();
			if (lines.size() == 2) {
				if (maps.count(lines[0]) > 0) {
					maps[lines[0]].push_back(lines[1]);
				}
				else {
					lineGroup.push_back(lines[1]);
					maps.insert(std::make_pair(lines[0], lineGroup));
				}
				lines.clear();
				lineGroup.clear();
			}
		}
	}

	fieldSize = fSize + 1;
	return maps;
}

void initializeField(std::vector<int> &maps, int fieldSize) {

	for (auto i = 0; i < fieldSize * fieldSize; i++) {
		maps.push_back(0);
	}
}

void buildMap(std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> location,
		std::vector<int> &maps, uint32_t fieldSize) {
	int xHigh = 0; 
	int xLow = 0;
	int yLow = 0;
	int yHigh = 0;

	for (auto it = location.begin(); it != location.end(); it++) {
		for (auto e : it->second) {
			if (it->first.first == e.first) {
				if (it->first.second > e.second) {
					xLow = e.second;
					xHigh = it->first.second;
				}
				else {
					xLow = it->first.second;
					xHigh = e.second;
				}

				for (auto j = xLow; j <= xHigh; j++) {
					maps[it->first.first + fieldSize * j]++;	
				}
			}

			if (it->first.second == e.second) {
				if (it->first.first > e.first) {
					xLow = e.first;
					xHigh = it->first.first;
				}
				else {
					xLow = it->first.first;
					xHigh = e.first;
				}

				for (auto j = xLow; j <= xHigh; j++) {
					maps[j + fieldSize * it->first.second]++;
				}
			}

			if ((it->first.first - e.first) * (it->first.first - e.first) ==
			     (it->first.second - e.second) * (it->first.second - e.second)) {
				if (it->first.first > e.first) {
					xLow = e.first;
					yLow = e.second; 
					xHigh = it->first.first;
					yHigh = it->first.second;
				}
				else {
					xLow = it->first.first;
					yLow = it->first.second;
					xHigh = e.first;
					yHigh = e.second;
				}
				if ((it->first.first - e.first) * (it->first.second - e.second) > 0) {

					int shift = 0;
					for (auto j = xLow; j <= xHigh; j++) {
						maps[j + fieldSize * (yLow + shift)]++;
						shift++;
					}
				}
				else {
					int shift = 0;
					for (auto j = xLow; j <= xHigh; j++) {
						maps[j + fieldSize * (yLow - shift)]++;
						shift++;
					}
				}
			}
		}
	}
}

uint32_t getNumberOfOverlaps(std::vector<int> maps) {

	uint32_t overlaps = 0;
	for (auto loc : maps) {
		if (loc > 1) overlaps++;
	}

	return overlaps;
}

void showMap(std::vector<int> maps, uint32_t fieldSize) {
	auto i = 0;

	for (auto e : maps) {
		std::cout << e << ", ";
		i++;
		if (i == fieldSize) {
			i = 0;
			std::cout << "\r\n";
		}
	}
}

int main() {

	
	std::map<std::pair<int, int>, std::vector<std::pair<int, int>>> location;
	uint32_t fieldSize;
	std::vector<int> maps;

	location = parseInput("input.txt", fieldSize);
	//display(location);
	initializeField(maps, fieldSize);
	buildMap(location, maps, fieldSize);
	//showMap(maps, fieldSize);
	std::cout << "Number of overlaps = " << getNumberOfOverlaps(maps) << std::endl;
	

	return 0;
}

