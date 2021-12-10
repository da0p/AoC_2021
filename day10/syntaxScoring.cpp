#include <functional>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <unordered_set>
#include <algorithm>

std::map<char, uint64_t> incorrectSyntaxPointTable {
	{')', 3},
	{']', 57},
	{'}', 1197},
	{'>', 25137}
};

std::map<char, uint64_t> incompleteSyntaxPointTable {
	{')', 1},
	{']', 2},
	{'}', 3},
	{'>', 4}
};

std::map<char, char> forwardMatchTable {
	{'(', ')'},
	{'{', '}'},
	{'[', ']'},
	{'<', '>'}
};

std::map<char, char> reverseMatchTable {
	{')', '('},
	{'}', '{'},
	{']', '['},
	{'>', '<'}
};

std::unordered_set<char> openBracketSet {'{', '[', '<', '('};

void display(std::vector<char> incorrectBrackets) {
	for (auto c : incorrectBrackets) 
		std::cout << c << ",";
	std::cout << std::endl;
}

std::vector<std::string> parseInput(std::string inputFileName) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString;
	std::vector<std::string> brackets;
	int r = 0;

	while (inputFile >> unusedString) {
			brackets.push_back(unusedString);
	}

	return	brackets; 
}

std::vector<char> findIncorrectBrackets(std::vector<std::string> brackets) {

	std::vector<char> incorrectBrackets;
	std::stack<char> openBrackets;

	for (auto s : brackets) {
		for (auto c : s) {
			if (openBracketSet.find(c) != openBracketSet.end()) {
				openBrackets.push(c);
			}
			else {
				if (reverseMatchTable[c] != openBrackets.top()) {
					incorrectBrackets.push_back(c);
					break;
				}
				else
					openBrackets.pop();
			}
		}
		while (!openBrackets.empty()) openBrackets.pop();
	}

	return incorrectBrackets;
}

std::vector<std::string> findIncompleteLines(std::vector<std::string> brackets) {

	std::stack<char> openBrackets;
	std::vector<std::string> incompleteLine;
	bool incomplete;

	for (auto s : brackets) {
		incomplete = true;
		for (auto c : s) {
			if (openBracketSet.find(c) != openBracketSet.end()) {
				openBrackets.push(c);
			}
			else {
				if (reverseMatchTable[c] != openBrackets.top()) {
					incomplete = false;
					break;
				}
				else
					openBrackets.pop();
			}
		}
		if (incomplete) incompleteLine.push_back(s);
		while (!openBrackets.empty())
			openBrackets.pop();
	}

	return incompleteLine;
}
uint64_t findTotalIncorrectSyntaxPoints(std::vector<std::string> brackets) {
	std::vector<char> incorrectBrackets = findIncorrectBrackets(brackets);
	uint64_t total = 0;

	for (auto b : incorrectBrackets) {
		total += incorrectSyntaxPointTable[b];
	}

	return total;
}

uint64_t findTotalIncompleteSyntaxPoints(std::vector<std::string> brackets) {
	std::vector<std::string> incompleteLines = findIncompleteLines(brackets);
	std::stack<char> openBrackets;
	std::vector<uint64_t> scores;
	uint64_t total = 0;

	for (auto iL : incompleteLines) {
		for (auto c : iL) {
			if (openBracketSet.find(c) != openBracketSet.end()) {
				openBrackets.push(c);
			}
			else {
				openBrackets.pop();	
			}
		}
		while(!openBrackets.empty()) {
			total = total * 5 + incompleteSyntaxPointTable[forwardMatchTable[openBrackets.top()]];
			openBrackets.pop();
		}

		scores.push_back(total);
		total = 0;
	}

	std::sort(scores.begin(), scores.end());

	return scores[scores.size() / 2];
}

int main() {

	std::vector<std::string> brackets = parseInput("input.txt");

	std::cout << "Incorrect Syntax Score = " << findTotalIncorrectSyntaxPoints(brackets) << std::endl;

	std::cout << "Incomplete Syntax Score = " << findTotalIncompleteSyntaxPoints(brackets) << std::endl;
	return 0;
}
