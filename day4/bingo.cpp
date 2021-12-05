#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

constexpr uint32_t TABLE_SIZE = 5;

void displayTable(std::vector<int> table) {
	uint32_t i = 0;
	std::cout << "{";
	for (auto e : table) {
		std::cout << e << ", ";
		i++;
		if (i == TABLE_SIZE) {
			std::cout << "\r\n";
			i = 0;
		}
	}
	std::cout << "}" << "\r\n\r\n" << std::endl;
}

void displayTable(std::vector<bool> table) {
	uint32_t i = 0;
	std::cout << "{";
	for (auto e : table) {
		std::cout << e << ", ";
		i++;
		if (i == TABLE_SIZE) {
			std::cout << "\r\n";
			i = 0;
		}
	}
	std::cout << "}" << "\r\n\r\n" << std::endl;
}

void displayTables(std::vector<std::vector<int>> tables) {
	for (auto table : tables) {
		displayTable(table);
	}
}


bool checkTable(std::vector<int> A, int val) {

	auto rowsCheck = 0;
	auto colsCheck = 0;

	for (auto i = 0; i < TABLE_SIZE; i++) {
		for (auto j = 0; j < TABLE_SIZE; j++) {
			if (A[TABLE_SIZE * i + j] == val) rowsCheck++;
			if (A[i + TABLE_SIZE * j] == val) colsCheck++;
		}
		if (rowsCheck == TABLE_SIZE) return true;
		if (colsCheck == TABLE_SIZE) return true;
		rowsCheck= 0;
		colsCheck = 0;
	}

	return false;
}

void checkTables(std::vector<std::vector<int>> tables, int val, 
			     std::vector<std::vector<int>> &winningTables, 
			     std::vector<int> &index, std::vector<bool> &won) {
	int i = 0;
	winningTables.clear();
	index.clear();
	for (auto table : tables) {
		if (!won[i]) {
			if (checkTable(table, val)) {
				winningTables.push_back(table);
				index.push_back(i);
				won[i] = true;
			}
		}
		i++;
	}
}

void replaceNumber(std::vector<std::vector<int>> &A, int draw, 
		   std::vector<bool> won, int val) {

	auto i = 0;
	for (auto &table : A) {
		if (!won[i]) {
			for (auto &e : table) {
				if (e == draw) e = val;
			}
		}
		i++;
	}
}

uint32_t score(std::vector<int> table, int draw, int val) {
	uint32_t unmarkedSum = 0;
	for (auto e : table) {
		if (e != val) {
			unmarkedSum += e;
		}
	}

	return unmarkedSum * draw;
}

uint32_t findFirstWinningTable(std::vector<std::vector<int>> &tables, std::vector<int> randomNumbers, int val) {
	std::vector<std::vector<int>> winningTables;
	std::vector<int> firstWinningTable;
	std::vector<int> index;
	std::vector<bool> won (tables.size());
	int winningDraw;

	std::fill(won.begin(), won.end(), false);
	for (auto randomNumber : randomNumbers) {
		replaceNumber(tables, randomNumber, won, -1);
		checkTables(tables, -1, winningTables, index, won);
		if (!index.empty()) {
			winningDraw = randomNumber;
			firstWinningTable = tables[index[0]];
			break;
		}
	}

	displayTable(firstWinningTable);

	auto winningScore =  score(firstWinningTable, winningDraw, -1);

	return winningScore;

}

uint32_t findLastWinningTable(std::vector<std::vector<int>> &tables, std::vector<int> randomNumbers, int val) {
	std::vector<std::vector<int>> winningTables;
	std::vector<int> lastWinningTable;
	std::vector<int> index;
	std::vector<bool> won (tables.size());

	auto i = 0;
	int winningDraw;
	std::fill(won.begin(), won.end(), false); 

	for (auto randomNumber : randomNumbers) {
		replaceNumber(tables, randomNumber, won, -1);
		checkTables(tables, -1, winningTables, index, won);
		if (!winningTables.empty()) {
			lastWinningTable = winningTables.back();
			winningDraw = randomNumber;
		}
	}
	
	displayTable(lastWinningTable);
	auto winningScore = score(lastWinningTable, winningDraw, -1);

	return winningScore;
}

int main() {

	std::ifstream inputFile("input.txt", std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<int> randomNumbers;
	std::vector<std::vector<int>> tables;
	std::vector<int> table;
	std::string unusedString;
	std::string token;
	int element;
	uint32_t index = 0;


	inputFile >> unusedString;
	std::istringstream ss(unusedString);

	while (std::getline(ss, token, ',')) {
		randomNumbers.push_back(stoi(token));
	}

	while (inputFile >> element) {
		table.push_back(element);	
		index++;
		if (index == TABLE_SIZE * TABLE_SIZE) {
			tables.push_back(table);
			table.clear();
			index = 0;
		}
	}

	//displayTables(tables);
	std::cout << "-------------------------" << std::endl;
	std::cout << findFirstWinningTable(tables, randomNumbers, -1) << std::endl;
	std::cout << "-------------------------" << std::endl;
	std::cout << findLastWinningTable(tables, randomNumbers, -1) << std::endl;
	std::cout << "-------------------------" << std::endl;
	//displayTables(tables);

	return 0;
}

