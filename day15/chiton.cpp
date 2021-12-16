#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <stdint.h>
#include <bits/stdc++.h>

typedef struct Vertex {
	uint32_t row;
	uint32_t col;
	int risk;
} Vertex;

std::vector<std::vector<Vertex>> parseInput(std::string inputFileName) 
{
	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::vector<std::vector<Vertex>> chito;
	std::vector<Vertex> tmp;
	std::string unusedString;
	uint32_t row = 0, col = 0;

	while (inputFile >> unusedString) {
		for (auto c : unusedString) {
			Vertex v {row, col, c - 48};
			tmp.push_back(v);
			col++;
		}
		chito.push_back(tmp);
		tmp.clear();
		col = 0;
		row++;
	}

	return chito;
}

uint32_t findLowestRiskPath(std::vector<std::vector<Vertex>> chito)
{
	auto row = chito.size();
	auto col = chito[0].size();
	std::vector<std::vector<int>> minPath (row, std::vector<int>(col));

	auto comparator = [](Vertex v1, Vertex v2){ return v1.risk >= v2.risk; };
	std::priority_queue<Vertex, std::vector<Vertex>, decltype(comparator)> vertices(comparator);

	for (auto r = 0; r < row; r++) {
		for (auto c = 0; c < col; c++) {
			minPath[r][c] = INT32_MAX;
		}
	}

	vertices.push({0, 0, 0});
	minPath[0][0] = 0;

	while (!vertices.empty()) {
	
		auto v = vertices.top();
		vertices.pop();
		auto r = v.row;
		auto c = v.col;

		if (r > 0 and minPath[r][c] + chito[r - 1][c].risk < minPath[r - 1][c]) {
			minPath[r - 1][c] = minPath[r][c] + chito[r - 1][c].risk;
			vertices.push({r - 1, c, minPath[r - 1][c]});
		}

		if (c > 0 and minPath[r][c] + chito[r][c - 1].risk < minPath[r][c - 1]) {
			minPath[r][c - 1] = minPath[r][c] + chito[r][c - 1].risk;
			vertices.push({r, c - 1, minPath[r][c - 1]});
		}

		if (r < row - 1 and minPath[r][c] + chito[r + 1][c].risk < minPath[r + 1][c]) {
			minPath[r + 1][c] = minPath[r][c] + chito[r + 1][c].risk;
			vertices.push({r + 1, c, minPath[r + 1][c]});
		}

		if (c < col - 1 and minPath[r][c + 1] + chito[r][c + 1].risk < minPath[r][c + 1]) {
			minPath[r][c + 1] = minPath[r][c] + chito[r][c + 1].risk;
			vertices.push({r, c + 1, minPath[r][c + 1]});
		}
	}
	
	return minPath[row - 1][col - 1];
}

void increase(std::vector<std::vector<Vertex>> &chito, uint32_t i) {
	auto row = chito.size();
	auto col = chito[0].size();

	for (auto r = 0; r < row; r++) {
		for (auto c = 0; c < col; c++) {
			chito[r][c].risk = chito[r][c].risk + i;
			if (chito[r][c].risk > 9) chito[r][c].risk -= 9;
		}
	}
}

std::vector<std::vector<Vertex>> expandFullMap(std::vector<std::vector<Vertex>> chito)
{
	auto row = chito.size();
	auto col = chito[0].size();

	std::vector<std::vector<Vertex>> fullChito (row * 5, std::vector<Vertex> (col * 5));	
	auto fullChitoRow = fullChito.size();
	auto fullChitoCol = fullChito[0].size();

	std::vector<std::vector<std::vector<Vertex>>> expanded;

	for (auto i = 0; i < 25; i++) {
		expanded.push_back(chito);
	}

	for (auto i = 0; i < 25; i++) {
		increase(expanded[i], i / 5 + i % 5);
	}

	for (auto r = 0; r < fullChitoRow; r++) {
		for (auto c = 0; c < fullChitoCol; c++) {
			fullChito[r][c] = expanded[(r / row) * 5 + c / col][r % row][c % col]; 
		}
	}

	return fullChito;
}

void displayChito(std::vector<std::vector<Vertex>> chito) {

	auto row = chito.size();
	auto col = chito[0].size();

	for (auto r = 0; r < row; r++) {
		for (auto c = 0; c < col; c++) {
			std::cout << chito[r][c].risk;
		}
		std::cout << std::endl;
	}
}

int main() {

	auto chito = parseInput("input.txt");

	std::cout << findLowestRiskPath(chito) << std::endl;

	//displayChito(chito);

	auto fullChito = expandFullMap(chito);

	//displayChito(fullChito);
	//
	std::cout << findLowestRiskPath(fullChito) << std::endl;

	return 0;
}
