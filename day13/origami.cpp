#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <numeric>

std::pair<std::vector<int>, std::vector<std::pair<std::string, int>>> parseInput(std::string inputFileName, int &maxX, int &maxY) 
{
	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString, token;
	std::vector<std::pair<int, int>> dotCoordinates;
	std::vector<int> coordinates (2);
	std::vector<std::pair<std::string, int>> instructions;
	std::string foldingAxis;
	int foldingCoordinate;
	maxX = 0, maxY = 0;
	auto i = 0;

	while (inputFile >> unusedString) {
		if (unusedString == "fold") break;
		std::istringstream ss(unusedString);
		while(std::getline(ss, token, ',')) {
			coordinates[i] = stoi(token);
			i++;
		}
		maxX = std::max(coordinates[0], maxX);
		maxY = std::max(coordinates[1], maxY);
		dotCoordinates.push_back(std::make_pair(coordinates[0], coordinates[1]));
		i = 0;
	}

	i = 0;
	while (inputFile >> unusedString) {
		if (unusedString == "fold") continue;
		if (unusedString == "along") continue;
		std::istringstream ss(unusedString);
		while(std::getline(ss, token, '=')) {
			if (i == 0) foldingAxis = token;
			else foldingCoordinate = stoi(token);
			i++;
		}
		instructions.push_back(std::make_pair(foldingAxis, foldingCoordinate));
		i = 0;
	}
	
	maxX = maxX % 2 == 0 ? maxX + 1 : maxX + 2; 
	maxY = maxY % 2 == 0 ? maxY + 1 : maxY + 2;
	std::cout << maxX << ", " << maxY << std::endl;
	std::vector<int> table (maxX * maxY);
	for (auto coordinate : dotCoordinates) {
		table[coordinate.first + coordinate.second * maxX] = 1;
	}

	return {table, instructions};
}

void displayTable(std::vector<int> table, int cols) {
	auto i = 0;
	std::cout << "-------------------------" << std::endl;
	for (auto e : table) {
		if (e == 1) std::cout << "#";
		else std::cout << ".";
		i++;
		if (i >= cols) {
			std::cout << "\r\n";
			i = 0;
		}
	}
	std::cout << "-------------------------" << std::endl;

}

void displayInstructions(std::vector<std::pair<std::string, int>> instructions) {
	for (auto ins : instructions) {
		std::cout << ins.first << " = " << ins.second;
	}
	std::cout << std::endl;
}

std::vector<int> foldAlongX(std::vector<int> table, int &row, int &col, int xAxis) {
	
	int foldedCol = xAxis;

	std::vector<int> foldedTable (row * foldedCol);

	auto startCol = 2 * foldedCol - col + 1;
	for (auto foldedTableIndex = 0; foldedTableIndex < foldedTable.size(); foldedTableIndex++) {
		int foldedTableRow = foldedTableIndex / foldedCol;
		int foldedTableCol = foldedTableIndex % foldedCol;

		if (foldedTableCol >= startCol) {
		foldedTable[foldedTableIndex] = 
			table[foldedTableRow * col + foldedTableCol] + table[foldedTableRow * col + 2 * foldedCol - foldedTableCol]; 
		}
		else {
			foldedTable[foldedTableIndex] = table[foldedTableIndex];
		}

		if (foldedTable[foldedTableIndex] > 1) foldedTable[foldedTableIndex] = 1;
	}

	col = foldedCol;

	return foldedTable;
}

std::vector<int> foldAlongY(std::vector<int> table, int &row, int &col, int yAxis) {
	int foldedRow = yAxis;

	std::vector<int> foldedTable (foldedRow * col);  

	auto startRow = 2 * foldedRow - row + 1;
	for (auto foldedTableIndex = 0; foldedTableIndex < foldedTable.size(); foldedTableIndex++) {
		int foldedTableRow = foldedTableIndex / col;
		int foldedTableCol = foldedTableIndex % col;

		if (foldedTableRow >= startRow) {
		foldedTable[foldedTableIndex] = 
			table[foldedTableIndex] + table[(2 * foldedRow - foldedTableRow) * col + foldedTableCol]; 
		}
		else {
			foldedTable[foldedTableIndex] = table[foldedTableIndex];
		}
		if (foldedTable[foldedTableIndex] > 1) foldedTable[foldedTableIndex] = 1;
	}

	row = foldedRow;

	return foldedTable;
}

uint32_t countDots(std::vector<int> table) {
	return std::accumulate(table.begin(), table.end(), 0);
}

uint32_t visibleDots(std::vector<int> table, int row, int col, std::vector<std::pair<std::string, int>> instructions) {
	auto firstInstruction = instructions[0];
	std::vector<int> foldedTable = table;
	int newCol = col, newRow = row;

	if (firstInstruction.first == "x") {
		foldedTable = foldAlongX(foldedTable, row, col, firstInstruction.second);
	}
	if (firstInstruction.first == "y") {
		foldedTable = foldAlongY(foldedTable, row, col, firstInstruction.second);
	}

	//displayTable(foldedTable, newCol);
	
	return countDots(foldedTable);
}

uint32_t decode(std::vector<int> table, int row, int col, std::vector<std::pair<std::string, int>> instructions) {
	std::vector<int> foldedTable = table;
	int newCol = col, newRow = row;

	for (auto ins : instructions) {
		if (ins.first == "x") {
			foldedTable = foldAlongX(foldedTable, newRow, newCol, ins.second);
		}
		if (ins.first == "y") {
			foldedTable = foldAlongY(foldedTable, newRow, newCol, ins.second);
		}
	}

	displayTable(foldedTable, newCol);
	
	return countDots(foldedTable);
}

int main() {

	int cols, rows;

	auto origami = parseInput("input.txt", cols, rows);

	std::vector<int> table;

	std::vector<std::pair<std::string, int>> instructions;

	table = origami.first;

	instructions = origami.second;

	//displayTable(table, cols);

	std::cout << "-------------------------------------------------" << std::endl;
	std::cout << visibleDots(table, rows, cols, instructions) << std::endl;

	std::cout << decode(table, rows, cols, instructions) << std::endl; 
	return 0;
}
