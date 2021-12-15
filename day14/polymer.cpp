#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>

std::pair<std::string, std::map<std::string, std::string>> parseInput(std::string inputFileName) 
{
	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string key, value, unusedString, templateString;
	std::map<std::string, std::string> pairInsertionRule;

	inputFile >> templateString;

	while (inputFile >> key >> unusedString >> value) {
		pairInsertionRule.insert(std::make_pair(key, value));
	}

	return {templateString, pairInsertionRule};
}

uint64_t polymerGrowth(std::pair<std::string, std::map<std::string, std::string>> polymer, uint64_t steps) {
	
	auto templateString = polymer.first;
	auto pairInsertionRule = polymer.second;	
	std::map<std::string, uint64_t> polymerString;
	std::map<std::string, uint64_t> growPolymerString;
	std::map<char, uint64_t> charCount;
	std::string tmp = "";

	for (auto i = 0; i < templateString.size() - 1; i++) {
		tmp += templateString[i];
		tmp += templateString[i + 1];
		if (polymerString.count(tmp) > 0) polymerString[tmp]++;
		else polymerString.insert(std::make_pair(tmp, 1));
		tmp = "";
	}

	for (auto c : templateString) {
		if (charCount.count(c) > 0) charCount[c]++;
		else charCount.insert(std::make_pair(c, 1));
	}

	auto i = 0;
	std::string splitedKey1, splitedKey2;
	while (i < steps) {
		for (auto it = polymerString.begin(); it != polymerString.end(); it++) {
			if (pairInsertionRule.count(it->first) > 0) {
				splitedKey1 += it->first[0];		
				splitedKey1 += pairInsertionRule[it->first];
				splitedKey2 += pairInsertionRule[it->first];
				splitedKey2 += it->first[1];
								
				if (growPolymerString.count(splitedKey1) > 0) 
					growPolymerString[splitedKey1] += it->second;
				else 
					growPolymerString.insert(std::make_pair(splitedKey1, it->second));

				if (growPolymerString.count(splitedKey2) > 0) 
					growPolymerString[splitedKey2] += it->second;
				else 
					growPolymerString.insert(std::make_pair(splitedKey2, it->second));	
				
				auto insertedString = pairInsertionRule[it->first];
				if (charCount.count(insertedString[0]) > 0) 
					charCount[insertedString[0]] += it->second;
				else 
					charCount.insert(std::make_pair(insertedString[0], 1));
			} 
			else {
				auto oldKey = std::make_pair(it->first, it->second);
				growPolymerString.insert(oldKey);
			}
			splitedKey1 = "";
			splitedKey2 = "";
		}

		polymerString = growPolymerString;
		growPolymerString.clear();
		i++;
	}

	auto first = charCount.begin();
	uint64_t minValue = first->second;
	uint64_t maxValue = 0;

	for (auto it = charCount.begin(); it != charCount.end(); it++) {
		if (it->second < minValue) minValue = it->second;
		if (it->second > maxValue) maxValue = it->second;
	}

	return maxValue - minValue;
}

int main() {

	auto polymer = parseInput("input.txt");

	std::cout << "Task 1 " << polymerGrowth(polymer, 10) << std::endl;

	std::cout << "Task 2: " << polymerGrowth(polymer, 40) << std::endl;


	return 0;
}
