#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <unordered_set>
#include <map>
#include <algorithm>
#include <string_view>
#include <cctype>

constexpr auto START = "start";
constexpr auto END = "end";

bool isSmallCave(std::string cave) {
	for (auto c : cave) {
		if (std::islower(c) == 0) return false;
	}

	return true;
}

bool isBigCave(std::string cave) {
	return !isSmallCave(cave);
}

bool isEnd(std::string cave) {
	if (cave == END) return true;
	else return false;
}

bool isStart(std::string cave) {
	if (cave == START) return true;
	else return false;
}

void displayCaveConnection (std::map<std::string, std::vector<std::string>> paths) {
	std::cout << "-------------------------------" << std::endl;
	for (auto it = paths.begin(); it != paths.end(); it++) {
		for (auto cave : it->second) {
			std::cout << it->first << ", " << cave << std::endl;
		}
	}
	std::cout << "-------------------------------" << std::endl;
}

void displayPaths (std::vector<std::deque<std::string>> goodPaths) {
	for (auto path : goodPaths) {
		for (auto cave : path) {
			std::cout << cave << ", ";
		}
		std::cout << std::endl;
	}
}

std::map<std::string, std::vector<std::string>> parseInput(std::string inputFileName) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString, token;
	std::vector<std::string> caveConnection;
	std::map<std::string, std::vector<std::string>> paths;

	while (inputFile >> unusedString) {
		std::istringstream ss(unusedString);
		while(std::getline(ss, token, '-')) 
			caveConnection.push_back(token);
		
		if (paths.count(caveConnection[0]) > 0) {
			paths[caveConnection[0]].push_back(caveConnection[1]);
		}
		else {
			paths.insert({caveConnection[0], {caveConnection[1]}}); 
		}

		if (paths.count(caveConnection[1]) > 0) {
			paths[caveConnection[1]].push_back(caveConnection[0]);
		}
		else {
			paths.insert({caveConnection[1], {caveConnection[0]}});
		}
		caveConnection.clear();
	}

	return paths;
}

void displayExploredPath(std::deque<std::string> exploredPath) {
	
	for (auto cave : exploredPath) 
		std::cout << cave << ", ";
	std::cout << std::endl;
}

bool oneSmallCaveMultipleTimeVisited(std::map<std::string, uint32_t> seachedSmallCaves, uint32_t maxSmallCaveVisits) {
	for (auto it = seachedSmallCaves.begin(); it != seachedSmallCaves.end(); it++) {
		if (it->second == maxSmallCaveVisits) return true;
	}
	return false;
}

void exploreCave(std::string cave, uint32_t maxSmallCaveVisits, std::map<std::string, std::vector<std::string>> paths, 
		std::map<std::string, uint32_t> &seachedSmallCaves, std::deque<std::string> &exploredPath, std::vector<std::deque<std::string>> &goodPaths) {
	
	if (isEnd(cave)) {
		goodPaths.push_back(exploredPath);
		return;
	}

	if (seachedSmallCaves.count(cave) > 0 and isStart(cave)) return;

	if (seachedSmallCaves.count(cave) > 0 and 
	    seachedSmallCaves[cave] >= 1 and
	    oneSmallCaveMultipleTimeVisited(seachedSmallCaves, maxSmallCaveVisits)) return;

	if (paths.count(cave) == 0) return;

	if (isSmallCave(cave)) {
		if (seachedSmallCaves.count(cave) > 0) seachedSmallCaves[cave]++;
		else seachedSmallCaves.insert(std::make_pair(cave, 1));
	}
	
	for (auto cv : paths[cave]) {
		exploredPath.push_back(cv);
		exploreCave(cv, maxSmallCaveVisits, paths, seachedSmallCaves, exploredPath, goodPaths);
		exploredPath.pop_back();
	}

	if (seachedSmallCaves.count(cave) > 0 and seachedSmallCaves[cave] > 0)
		seachedSmallCaves[cave]--;

}

std::vector<std::deque<std::string>> explorePaths(std::map<std::string, std::vector<std::string>> paths) {
	std::map<std::string, uint32_t> seachedSmallCaves;
	std::deque<std::string> exploredPath;
	std::vector<std::deque<std::string>> goodPaths;

	if (paths.count(START) > 0) {
		auto startCaves = paths[START];
		for (auto startCave : startCaves) {
			exploredPath.push_back(START);
			exploredPath.push_back(startCave);
			seachedSmallCaves.insert(std::make_pair(START, 1));
			exploreCave(startCave, 1, paths, seachedSmallCaves, exploredPath, goodPaths);
			exploredPath.clear();	
			seachedSmallCaves.clear();
		}
	}
	return goodPaths;
}


std::vector<std::deque<std::string>> exploreDifferentPaths(std::map<std::string, std::vector<std::string>> paths) {
	std::map<std::string, uint32_t> seachedSmallCaves;
	std::deque<std::string> exploredPath;
	std::vector<std::deque<std::string>> goodPaths;

	if (paths.count(START) > 0) {
		auto startCaves = paths[START];
		for (auto startCave : startCaves) {
			exploredPath.push_back(START);
			exploredPath.push_back(startCave);
			seachedSmallCaves.insert(std::make_pair(START, 1));
			exploreCave(startCave, 2, paths, seachedSmallCaves, exploredPath, goodPaths);
			exploredPath.clear();	
			seachedSmallCaves.clear();
		}
	}
	return goodPaths;
}


uint32_t countPaths(std::map<std::string, std::vector<std::string>> paths) {

	auto goodPaths = explorePaths(paths);

//	displayPaths(goodPaths);

	return goodPaths.size();
}

uint32_t countPathsMultipleVistsToSmallCaves(std::map<std::string, std::vector<std::string>> paths) {
	auto goodPaths = exploreDifferentPaths(paths);	

	//displayPaths(goodPaths);

	return goodPaths.size();
}

int main() {

	std::map<std::string, std::vector<std::string>>  paths = parseInput("input.txt");

	displayCaveConnection(paths);

	std::cout << countPaths(paths) << std::endl;

	std::cout << countPathsMultipleVistsToSmallCaves(paths) << std::endl;

	return 0;
}

