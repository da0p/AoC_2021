#include <iostream>
#include <fstream>
#include <sstream>
#include <variant>
#include <memory>
#include <vector>
#include <string_view>

struct SnailFishNumber {
	uint64_t value;
	SnailFishNumber *parent;
	std::unique_ptr<SnailFishNumber> left;
	std::unique_ptr<SnailFishNumber> right;
};

SnailFishNumber insertSnailFishNumber(std::string::iterator &begin,
			   std::string::iterator &end, SnailFishNumber *parent)
{
	SnailFishNumber sF;

	if (begin >= end) return sF;

	begin++;
	sF.parent = parent;
	//std::cout << "*begin = " << *begin << std::endl;
	sF.left = std::make_unique<SnailFishNumber>();
	if (*begin == '[') {
		*sF.left = insertSnailFishNumber(begin, end, &sF);
	}
	else {
		sF.left->value = *begin - 48;
		//std::cout << "*begin = " << *begin << ", "
		//	<< "value = " << sF.left->value << std::endl;

	}

	begin += 2;
	sF.right = std::make_unique<SnailFishNumber>();
	if (*begin == '[') {
		*sF.right = insertSnailFishNumber(begin, end, &sF);
	}
	else {
		sF.right->value = *begin - 48;
		//std::cout << "*begin = " << *begin << ", "
		//	<< "value = " << sF.right->value << std::endl;
	}
	begin++;

	return sF;
}

std::vector<SnailFishNumber> parseInput(std::string inputFileName) 
{

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString;
	std::vector<SnailFishNumber> snailFishes;
	SnailFishNumber *parent = nullptr;

	while (inputFile >> unusedString) {
		auto begin = unusedString.begin();
		auto end = unusedString.end();
		snailFishes.push_back(insertSnailFishNumber(begin, end, parent));
	}

	return snailFishes;
}

void displaySnailFishNumber(SnailFishNumber &sF) {
	if (sF.left == nullptr and sF.right == nullptr) {
		std::cout << sF.value;
		return;
	}

	std::cout << "[";
	displaySnailFishNumber(*sF.left);
	std::cout << ", ";
	displaySnailFishNumber(*sF.right);
	std::cout << "]";
}

void displaySnailFishNumbers(std::vector<SnailFishNumber> &snailFishes) {

	for (auto &sF : snailFishes) {
		displaySnailFishNumber(sF);
	}
}

int main() {

	auto snailFishes =  parseInput("input4.txt");

	displaySnailFishNumbers(snailFishes);

	return 0;
}
