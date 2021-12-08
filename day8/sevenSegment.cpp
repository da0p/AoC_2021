#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>

std::vector<std::vector<std::vector<std::string>>> parseInput(std::string inputFileName) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString;
	std::vector<std::string> digitSet;
	std::vector<std::string> trainingSet;
	std::vector<std::vector<std::vector<std::string>>> digitNumbers; 
	uint32_t i = 0;

	while (inputFile >> unusedString) {
		if (i > 10) {
			digitSet.push_back(unusedString);
		}
		else if (i < 10) {
			trainingSet.push_back(unusedString);
		}
		i++;
		if (i > 14) {
			digitNumbers.push_back({trainingSet, digitSet});
			digitSet.clear();
			trainingSet.clear();
			i = 0;
		}
	}
	
	return	digitNumbers; 
}

std::set<char> strToCharSet(std::string digit) {
	
	std::set<char> charSet;

	for (auto c : digit) {
		charSet.insert(c);
	}

	return charSet;
}

std::set<char> strAnd(std::string digit1, std::string digit2) {
	std::set<char> commonCharSet;

	for (auto c : digit1) 
		commonCharSet.insert(c);

	for (auto c : digit2)
		commonCharSet.insert(c);

	return commonCharSet;
}

std::vector<std::string> lookForDeterministicNumbers(std::vector<std::string> trainingSet) {
	std::vector<std::string> res (4);
	for (auto digit : trainingSet) {
		if (digit.length() == 2) res[0] = digit;
		else if (digit.length() == 3) res[2] = digit;
		else if (digit.length() == 4) res[1] = digit;
		else if (digit.length() == 7) res[3] = digit;
	}

	return res;
}

std::vector<std::string> lookForFiveSegmentNumbers(std::vector<std::string> trainingSet) {
	std::vector<std::string> res;
	for (auto digit : trainingSet) {
		if (digit.length() == 5) res.push_back(digit);
	}

	return res;
}

std::vector<std::string> lookForSixSegmentNumbers(std::vector<std::string> trainingSet) {
	std::vector<std::string> res;
	for (auto digit : trainingSet) {
		if (digit.length() == 6) res.push_back(digit);
	}

	return res;
}

std::map<std::set<char>, uint32_t> buildDecoder(std::vector<std::string> trainingSet) {
	std::vector<std::string> dNumbers = lookForDeterministicNumbers(trainingSet);
	std::vector<std::string> fiveSegmentNumbers = lookForFiveSegmentNumbers(trainingSet);
	std::vector<std::string> sixSegmentNumbers = lookForSixSegmentNumbers(trainingSet);
	std::map<std::set<char>, uint32_t> decoder;
	std::vector<std::string> numberString (10);
	
	numberString[1] = dNumbers[0];
	auto numberOneSet = std::make_pair(strToCharSet(numberString[1]), 1);
	decoder.insert(numberOneSet);

	numberString[4] = dNumbers[1];
	auto numberFourSet = std::make_pair(strToCharSet(numberString[4]), 4);
	decoder.insert(numberFourSet);

	numberString[7] = dNumbers[2];
	auto numberSevenSet = std::make_pair(strToCharSet(numberString[7]), 7);
	decoder.insert(numberSevenSet);
	
	numberString[8] = dNumbers[3];
	auto numberEightSet = std::make_pair(strToCharSet(numberString[8]), 8);
	decoder.insert(numberEightSet);

	for (auto s : sixSegmentNumbers) {
		if (strAnd(s, numberString[1]) != strToCharSet(s)) {
			numberString[6] = s;
			auto numberSixSet = std::make_pair(strToCharSet(s), 6);
			decoder.insert(numberSixSet);
		}

		if (strAnd(s, numberString[4]) == strToCharSet(s)) {
			numberString[9] = s;
			auto numberNineSet = std::make_pair(strToCharSet(s), 9);
			decoder.insert(numberNineSet);
		}
	}

	for (auto s : fiveSegmentNumbers) {
		if (strAnd(s, numberString[1]) == strToCharSet(s)) {
			numberString[3] = s;
			auto numberThreeSet = std::make_pair(strToCharSet(s), 3);	
			decoder.insert(numberThreeSet);
		}

		if (strAnd(s, numberString[9]) == strToCharSet(numberString[8])) {
			numberString[2] = s;
			auto numberTwoSet = std::make_pair(strToCharSet(s), 2);
			decoder.insert(numberTwoSet);
		}

		if (strAnd(s, numberString[9]) == strToCharSet(numberString[9])) {
			numberString[5] = s;
			auto numberFiveSet = std::make_pair(strToCharSet(s), 5);
			decoder.insert(numberFiveSet);
		}
	}

	for (auto s : sixSegmentNumbers) {
		if (strAnd(s, numberString[5]) == strToCharSet(s)) {
			numberString[0] = s;
			auto numberZeroSet = std::make_pair(strToCharSet(s), 0);
			decoder.insert(numberZeroSet);
		}
	}

	return decoder;
}

uint32_t decode(std::vector<std::vector<std::vector<std::string>>> digitNumbers) {
	std::vector<std::string> trainingSet;
	std::vector<std::string> signalNumber;
	std::map<std::set<char>, uint32_t> decoder;
	std::vector<uint32_t> digit (4);
	uint32_t total = 0, i = 0;

	for (auto digitNumber : digitNumbers) {
		trainingSet = digitNumber[0];
		signalNumber = digitNumber[1];
		decoder = buildDecoder(trainingSet);
		for (auto outputDigit : signalNumber) {
			digit[i] = decoder[strToCharSet(outputDigit)];
			i++;
		}
		total += digit[0] * 1000 + digit[1] * 100 + digit[2] * 10 + digit[3];
		i = 0;
		digit.clear();
	}

	return total;
}

uint32_t countDistinctOutput(std::vector<std::vector<std::vector<std::string>>> digitNumbers) {

	uint32_t total = 0;

	for (auto digitNumber : digitNumbers) {
		for (auto d : digitNumber[1]) {
			if (d.length() == 2 or d.length() == 3 or
			d.length() == 4 or d.length() == 7) {
				total++;
			}
		}
	}

	return total;
}

void display(std::vector<std::vector<std::vector<std::string>>> digitNumbers) {
	for (auto digitNumber : digitNumbers) {
		std::cout << "{";
		for (auto d : digitNumber[0]) {
			std::cout << d << ",";
		}
		std::cout << "}, ";

		std::cout << "{";
		for (auto d : digitNumber[1]) {
			std::cout << d << ",";
		}
		std::cout << "}" << std::endl;
	}
}

int main() {

	std::vector<std::vector<std::vector<std::string>>> digitNumbers = parseInput("input.txt");

	std::cout << "Part 1: " << countDistinctOutput(digitNumbers) << std::endl;

	std::cout << "Part 2: " << decode(digitNumbers) << std::endl;

	return 0;
}
