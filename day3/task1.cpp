#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <array>

int main() {

	std::ifstream inputFile("input.txt", std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string binaryString;
	std::array<uint32_t, 12> binarySum{0};
	auto i = 1;
	auto maxNum = 0x0fff;
	auto totalNum = 0;

	while (inputFile >> binaryString) {
		for (auto bin : binaryString) {
			binarySum[12 - i] += bin - 48;
			i++;
		}
		i = 1;
		totalNum++;
	}

	auto gamma = 0;
	i = 1;
	for (auto n : binarySum) {
		if (n > totalNum / 2) gamma += i; 
		i = i * 2;
	}

	auto epsilon = maxNum - gamma;

	std::cout << "consumption = " << gamma * epsilon << std::endl;

	return 0;
}
