#include <cmath>
#include <iterator>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <variant>
#include <string_view>

class SnailFishNumber {

public:

	static std::vector<std::unique_ptr<SnailFishNumber>> parse(std::string inputFileName) {
		std::ifstream inputFile(inputFileName, std::ios::in);

		if (!inputFile) {
			std::cerr << "File could not be opened" << std::endl;
			exit(EXIT_FAILURE);
		}

		std::string unusedString;
		std::vector<std::unique_ptr<SnailFishNumber>> snailFishes;
		SnailFishNumber *parent = nullptr;

		while (inputFile >> unusedString) {
			auto begin = unusedString.begin();
			auto end = unusedString.end();
			snailFishes.push_back(std::move(insertSnailFishNumber(begin, end, parent)));
		}

		return snailFishes;
	}

	static std::vector<std::string> parseRaw(std::string inputFileName) {

		std::ifstream inputFile(inputFileName, std::ios::in);

		if (!inputFile) {
			std::cerr << "File could not be opened" << std::endl;
			exit(EXIT_FAILURE);
		}

		std::string unusedString;
		std::vector<std::string> snailFishes;

		while (inputFile >> unusedString) {
			snailFishes.push_back(unusedString);
		}

		return snailFishes;

	}

	static std::unique_ptr<SnailFishNumber> parseSingleLine(std::string line) {
		auto begin = line.begin();
		auto end = line.end();
		return insertSnailFishNumber(begin, end, nullptr);
	}

	static std::unique_ptr<SnailFishNumber> insertSnailFishNumber(std::string::iterator &begin,
								std::string::iterator &end,
								SnailFishNumber *parent) {
		auto sF = std::make_unique<SnailFishNumber>();
		if (begin >= end) return sF;

		begin++;
		sF->parent = parent;

		if (*begin == '[') {
			sF->left.emplace<std::unique_ptr<SnailFishNumber>>(insertSnailFishNumber(begin, end, sF.get()));
		}
		else {
			sF->left.emplace<uint64_t>(*begin - 48);
		}

		begin += 2;
		if (*begin == '[') {
			sF->right.emplace<std::unique_ptr<SnailFishNumber>>(insertSnailFishNumber(begin, end, sF.get()));
		}
		else {
			sF->right.emplace<uint64_t>(*begin - 48);
		}
		begin++;

		return sF;

	}
	static std::unique_ptr<SnailFishNumber> add(std::unique_ptr<SnailFishNumber> left,
						std::unique_ptr<SnailFishNumber> right) {
		auto num = std::make_unique<SnailFishNumber>();
		left->parent = num.get();
		right->parent = num.get();
		num->left = std::move(left);
		num->right = std::move(right);
		return num;
	}

	uint64_t magnitude() {
		uint64_t sum = 0;

		if (left.index() == 0) {
			sum += 3 * std::get<0>(left);
		}
		else {
			sum += 3 * std::get<1>(left)->magnitude();
		}

		if (right.index() == 0) {
			sum += 2 * std::get<0>(right);
		}
		else {
			sum += 2 * std::get<1>(right)->magnitude();
		}

		return sum;
	}

	void reduce() {
		while (1) {
			if (isExploded(1)) continue;
			if (isSplitted()) continue;
			return;
		}
	}

	void displaySnailFishNumber() {
		std::cout << "[";
		if (left.index() == 0) {
			std::cout << std::get<0>(left);
		}
		else {
			std::get<1>(left)->displaySnailFishNumber();
		}

		std::cout << ",";

		if (right.index() == 0) {
			std::cout << std::get<0>(right);
		}
		else {
			std::get<1>(right)->displaySnailFishNumber();
		}

		std::cout << "]";
	}
private:

	bool isExploded(uint64_t depth) {
		if (depth == 4) {
			if (left.index() == 1) {
				left = std::get<1>(left)->explode();
				return true;
			}

			if (right.index() == 1) {
				right = std::get<1>(right)->explode();
				return true;
			}
		}

		if (left.index() == 1 and std::get<1>(left)->isExploded(depth + 1))
			return true;
		if (right.index() == 1 and std::get<1>(right)->isExploded(depth + 1))
			return true;

		return false;
	}

	bool isSplitted() {
		if (left.index() == 0 and std::get<0>(left) >= 10) {
			left = split(std::get<0>(left));
			return true;
		}
		if (left.index() == 1 and std::get<1>(left)->isSplitted()) return true;

		if (right.index() == 0 and std::get<0>(right) >= 10) {
			right = split(std::get<0>(right));
			return true;
		}
		if (right.index() == 1 and std::get<1>(right)->isSplitted()) return true;

		return false;
	}

	uint64_t explode() const {
		uint64_t *leftOfPair = findLeft();
		if (leftOfPair != nullptr) *leftOfPair += std::get<0>(left);

		uint64_t* rightOfPair = findRight();
		if (rightOfPair != nullptr) *rightOfPair += std::get<0>(right);

		return 0;
	}

	std::unique_ptr<SnailFishNumber> split(uint64_t value) {
		auto sf = std::make_unique<SnailFishNumber>();
		sf->parent = this;
		sf->left = static_cast<uint64_t>(std::floor(static_cast<double>(value) / 2.0));
		sf->right = static_cast<uint64_t>(std::ceil(static_cast<double>(value) / 2.0));

		return sf;
	}

	uint64_t* leftMost() {
		if (left.index() == 0) return &std::get<0>(left);
		return std::get<1>(left)->leftMost();
	}

	uint64_t* rightMost() {
		if (right.index() == 0) return &std::get<0>(right);
		return std::get<1>(right)->rightMost();
	}

	uint64_t* findLeft() const {
		if (parent == nullptr) return nullptr;

		if (parent->left.index() == 1 and std::get<1>(parent->left).get() ==this) {
			return parent->findLeft();
		}

		if (parent->left.index() == 0) return &std::get<0>(parent->left);
		return std::get<1>(parent->left)->rightMost();
	}

	uint64_t* findRight() const {
		if (parent == nullptr) return nullptr;

		if (parent->right.index() == 1 and std::get<1>(parent->right).get() == this) {
			return parent->findRight();
		}

		if (parent->right.index() == 0) return &std::get<0>(parent->right);
		return std::get<1>(parent->right)->leftMost();
	}
	
	SnailFishNumber* parent = nullptr;
	std::variant<uint64_t, std::unique_ptr<SnailFishNumber>> left;
	std::variant<uint64_t, std::unique_ptr<SnailFishNumber>> right;
};

uint64_t sumAll(std::vector<std::unique_ptr<SnailFishNumber>> snailFishes) {

	auto result = std::move(snailFishes[0]);
	for (uint64_t i = 1; i < snailFishes.size(); i++) {
		//sF->displaySnailFishNumber();
		result = SnailFishNumber::add(std::move(result), std::move(snailFishes[i]));
		result->reduce();
	}
	
	return result->magnitude();
}

uint64_t findMaxSum(std::string inputFileName) {
	uint64_t maxSum = 0;

	auto snailFishes = SnailFishNumber::parseRaw(inputFileName);
	uint64_t size = snailFishes.size();
	for (uint64_t i = 0; i < size; i++) {
		for (uint64_t j = 0; j < size; j++) {
			if (i != j) {
				auto sF1 = SnailFishNumber::parseSingleLine(snailFishes[i]);
				auto sF2 = SnailFishNumber::parseSingleLine(snailFishes[j]);
				auto result = SnailFishNumber::add(std::move(sF1),
								   std::move(sF2));	
				result->reduce();
				maxSum = std::max(maxSum, result->magnitude());
			}
		}
	}

	return maxSum;
}


int main() {
	auto snailFishes = SnailFishNumber::parse("input.txt");	

	std::cout << "Part 1: " << sumAll(std::move(snailFishes)) << std::endl;

	std::cout << "Part 2: " << findMaxSum("input.txt") << std::endl;
}
