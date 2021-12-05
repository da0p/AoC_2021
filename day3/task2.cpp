#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <array>
#include <vector>


void display(std::vector<std::string> in, std::string noti) {
	std::cout << noti << ":" <<std::endl;
	for (auto s : in)
		std::cout << s << std::endl;
	std::cout << "------------------"<< std::endl;
}

uint32_t findOxyGeneratorRating(std::vector<std::string> binaryStrings) {
	auto index = 0;	
	auto maxNum = 0x0fff;
	auto totalNum = 0;
	auto sumAtIndex = 0;
	auto oxyGeneratorRating = 0;
	std::array<uint32_t, 12> binary{0};
	std::vector<std::string> oneLeading;
	std::vector<std::string> zeroLeading;
	std::vector<std::string> buffer{binaryStrings};

	while (index < 12) {
		for (auto bin : buffer) {
			sumAtIndex += bin[index] - 48;	
			totalNum++;
			if (bin[index] - 48) oneLeading.push_back(bin);
			else zeroLeading.push_back(bin);
		}
		
		if (sumAtIndex >= totalNum - sumAtIndex) {
			binary[11 - index] = 1;
			zeroLeading.clear();
			buffer = oneLeading;
			oneLeading.clear();
		}
		else {
			binary[11 - index] = 0;
			oneLeading.clear();
			buffer = zeroLeading;
			zeroLeading.clear();
		}

		totalNum = 0;
		sumAtIndex = 0;
		index++;
	}


	std::cout << buffer[0] << std::endl;
	index = 1;
	for (auto b : binary) {
		if (b) oxyGeneratorRating += index;
		index = index * 2;
	}

	return oxyGeneratorRating;
}

uint32_t findCO2SrubberRating(std::vector<std::string> binaryStrings) {
	auto index = 0;	
	auto maxNum = 0x0fff;
	auto totalNum = 0;
	auto sumAtIndex = 0;
	auto co2ScrubberRating = 0;
	std::array<uint32_t, 12> binary{0};
	std::vector<std::string> oneLeading;
	std::vector<std::string> zeroLeading;
	std::vector<std::string> buffer{binaryStrings};

	while (index < 12) {
		for (auto bin : buffer) {
			sumAtIndex += bin[index] - 48;	
			totalNum++;
			if (bin[index] - 48) {
				oneLeading.push_back(bin);
			}
			else {
				zeroLeading.push_back(bin);
			}
		}

		//display(oneLeading, "oneLeading");
		//display(zeroLeading, "zeroLeading");

		if (sumAtIndex == 0 | (sumAtIndex >= totalNum - sumAtIndex && sumAtIndex != totalNum)) {
			binary[11 - index] = 0;
			oneLeading.clear();
			buffer = zeroLeading;
			zeroLeading.clear();
		}
		else {
			binary[11 - index] = 1;
			zeroLeading.clear();
			buffer = oneLeading;
			oneLeading.clear();
		}
		
		totalNum = 0;
		sumAtIndex = 0;
		index++;
	}

	std::cout << buffer[0] << std::endl;

	index = 1;
	for (auto b : binary) {
		if (b) co2ScrubberRating += index;
		std::cout << b;
		index = index * 2;
	}

	std::cout << std::endl;
	return co2ScrubberRating;
}

int main() {

	std::ifstream inputFile("input.txt", std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string input;
	std::vector<std::string> inputSeries;
	std::vector<bool> marking;
	
	// prepare
	while (inputFile >> input) { 
		inputSeries.push_back(input);
	}

	std::cout << findOxyGeneratorRating(inputSeries) << std::endl;
	std::cout << findCO2SrubberRating(inputSeries) << std::endl;

	return 0;
}
