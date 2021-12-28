#include <iostream>
#include <fstream>
#include <regex>
#include <set>
#include <vector>

struct Cube {
	bool state {false};
	std::pair<int64_t, int64_t> xRange;
	std::pair<int64_t, int64_t> yRange;
	std::pair<int64_t, int64_t> zRange;
	int64_t sign {1};
};

Cube extractCube(std::string line) {
	std::regex re("^([\\w]+)\\sx=(-?\\d+)..(-?\\d+),y=(-?\\d+)..(-?\\d+),z=(-?\\d+)..(-?\\d+)$");

	std::smatch match;

	Cube cube;

	if (std::regex_search(line, match, re) == true) {
		if (match.size() != 8) {
			std::cerr << "Regex does something wrong!" << std::endl;
			exit(EXIT_FAILURE);
		}

		cube.state =  match.str(1) == "on" ? true : false;
		cube.xRange.first = std::stoi(match.str(2));
		cube.xRange.second = std::stoi(match.str(3));
		cube.yRange.first = std::stoi(match.str(4));
		cube.yRange.second = std::stoi(match.str(5));
		cube.zRange.first = std::stoi(match.str(6));
		cube.zRange.second = std::stoi(match.str(7));
	}

	return cube;
}

std::vector<Cube> parseInput(std::string inputFileName) {
	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "Could not open file" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string line;
	std::vector<Cube> cubes;

	while(std::getline(inputFile, line)) {
		auto cube = extractCube(line);
		cubes.push_back(cube);
	}

	return cubes;
}

int64_t cubeVolume(Cube cube) {

	return cube.sign * (cube.xRange.second - cube.xRange.first + 1) * 
	       (cube.yRange.second - cube.yRange.first + 1) *
	       (cube.zRange.second - cube.zRange.first + 1);
}

void displayCube(Cube cube) {
	std::cout << cube.state
			<< ", x = " << cube.xRange.first << ".." << cube.xRange.second
			<< ", y = " << cube.yRange.first << ".." << cube.yRange.second
			<< ", z = " << cube.zRange.first << ".." << cube.zRange.second
			<< std::endl;

}

void displayCubes(std::vector<Cube> cubes) {
	for (auto cube : cubes) {
		displayCube(cube);
	}
}

bool isCubeInRange(Cube cube, int64_t minVal, int64_t maxVal) {

	if (cube.xRange.first < minVal or cube.yRange.first < minVal or cube.zRange.first < minVal)
		return false;
	if (cube.xRange.second > maxVal or cube.yRange.second > maxVal or cube.zRange.second > maxVal)
		return false;

	return true;
}

bool intersect(Cube c1, Cube c2, Cube &c3) {
	
	int64_t minX = std::max(c1.xRange.first, c2.xRange.first);
	int64_t maxX = std::min(c1.xRange.second, c2.xRange.second);
	int64_t minY = std::max(c1.yRange.first, c2.yRange.first);
	int64_t maxY = std::min(c1.yRange.second, c2.yRange.second);
	int64_t minZ = std::max(c1.zRange.first, c2.zRange.first);
	int64_t maxZ = std::min(c1.zRange.second, c2.zRange.second);

	if (minX > maxX or minY > maxY or minZ > maxZ) return false;

	c3 = Cube{1, {minX, maxX}, {minY, maxY}, {minZ, maxZ}, (-1) * (c1.sign * c2.sign)};

	return true;
}

int64_t countCubeOn(std::vector<Cube> &cubes, int64_t minVal, int64_t maxVal) {

	int64_t volume = 0;
	Cube intersectedCube;
	std::vector<Cube> cuboid;

	for (uint64_t i = 0; i < cubes.size(); i++) {
		if (isCubeInRange(cubes[i], minVal, maxVal)) {
			uint64_t currentCuboidSize = cuboid.size();
			for (uint64_t j = 0; j < currentCuboidSize; j++) {
				if (intersect(cuboid[j], cubes[i], intersectedCube)) {
					//displayCube(intersectedCube);
					cuboid.push_back(intersectedCube);	
					volume += cubeVolume(intersectedCube);
				}
			}
	
			if (cubes[i].state) {
				volume += cubeVolume(cubes[i]); 
				cuboid.push_back(cubes[i]);
			}
		}
	}

	return volume;
}

int main(int argc, char *argv[]) {

	std::vector<Cube> cubes;
	if (argc < 2) {
		cubes = parseInput("input.txt");
	}
	else if (argc == 2) {
		cubes = parseInput(argv[1]);
	}
	else {
		std::cerr << "Opening file error" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::cout << "Part 1: \r\n" << countCubeOn(cubes, -50, 50) << std::endl;

	std::cout << "Part 2: \r\n" << countCubeOn(cubes, INT64_MIN, INT64_MAX) << std::endl; 

	return 0;
}
