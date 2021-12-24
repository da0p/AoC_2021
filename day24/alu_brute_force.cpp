#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <functional>
#include <vector>

std::ifstream openFile(std::string inputFileName) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "Could not open file" << std::endl;
		exit(EXIT_FAILURE);
	}

	return inputFile;
}

bool add(int &a, int b) {
	a += b;
	return true;
}

bool multiply(int &a, int b) {
	a *= b;
	return true;
}

bool divide(int &a, int b) {
	if (b != 0) {
		a /= b;
		return true;
	}
	return false;
}

bool mod(int &a, int b) {
	if (a < 0 or b <= 0) return false;
	else {
		a %= b;
		return true;
	}
}

bool equal(int &a, int b) {
	a == b ? a = 1 : a = 0;
	return true;
}

uint64_t pow10(int n) {
	uint64_t res = 1;
	while (n > 0) {
		res *= 10;
		n--;
	}

	return res;
}

int extract(uint64_t number, int pos) {
	
	return (number / pow10(pos)) % 10;
}

bool isValidMONAD(uint64_t number) {
	for (uint64_t i = 0; i < 14; i++) {
		return extract(number, i) == 0 ? false : true;
	}
	return false;
}

bool alu(std::string inputFileName, uint64_t number) {

	auto inputFile = openFile(inputFileName);

	int w = 0, x = 0, y = 0, z = 0;
	int pos = 13;

	std::map<std::string, std::function<bool (int&, int)>> toFunction {
		{"add", add},
		{"mul", multiply},
		{"div", divide},
		{"mod", mod},
		{"eql", equal}
	};

	std::map<std::string, int*> toVariable {
		{"x", &x},
		{"y", &y},
		{"z", &z},
		{"w", &w}
	};


	std::string line, token;
	std::vector<std::string> tmp;

	if (!isValidMONAD(number)) return false;

	while (std::getline(inputFile, line)) {
		
		std::istringstream ss(line);
		while (std::getline(ss, token, ' ')) {
			tmp.push_back(token);			
		}
		if (tmp[0] == "inp") {
			if (pos < 0) {
				std::cout << "pos can't be negative" << std::endl;
				return false;
			}
			*toVariable[tmp[1]] = extract(number, pos); 
			/*
			std::cout << "-----------------------" << std::endl;
			std::cout << "x = " << x << ", y = " << y 
				  << ", z = " << z << ", w = " << w << std::endl;
			std::cout << "-----------------------" << std::endl;
			*/
			pos--;
			
		}
		else {
			if (toVariable.count(tmp[2]) == 0) {
				if(!toFunction[tmp[0]](*toVariable[tmp[1]], stoi(tmp[2]))) {
					//std::cout << "Error perform " << tmp[0] << std::endl;
					return false;
				}
				/*
			std::cout << "-----------------------" << std::endl;
			std::cout << "x = " << x << ", y = " << y 
				  << ", z = " << z << ", w = " << w << std::endl;
			std::cout << "-----------------------" << std::endl;
			*/
			}
			else {
				if (!toFunction[tmp[0]](*toVariable[tmp[1]], *toVariable[tmp[2]])) {
					//std::cout << "Error with two variables" << std::endl;
					return false;
				}
			/*
			std::cout << "-----------------------" << std::endl;
			std::cout << "x = " << x << ", y = " << y 
				  << ", z = " << z << ", w = " << w << std::endl;
			std::cout << "-----------------------" << std::endl;
			*/
			}
		}
		tmp.clear();
	}
	std::cout << "z = " << z << std::endl;
	return z == 0;
}

uint64_t maxMONAD(std::string inputFileName) {

	//uint64_t high = pow10(14) - 1;
	uint64_t high =99999999999999;
	uint64_t low = 11111111111111;
	uint64_t max = 0;
/*
	while (high > low) {
		std::cout << "high = " << high << std::endl;
		if (!alu(inputFileName, high)) high = (high + low) / 2; 
		else {
			low = (high + low) / 2 + 1;
			max = high;
		}
	}
	*/
	while (high >= low) {
		//std::cout << "high = " << high << std::endl;
		if (alu(inputFileName, high)) {
			max = high;
			break;
		}
		high--;
	}
	//alu(inputFileName, high);

	return max;
}

int main() {

	std::cout << maxMONAD("input.txt") << std::endl;

	return 0;
}
