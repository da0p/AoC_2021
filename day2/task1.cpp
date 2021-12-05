#include <iostream>
#include <fstream>
#include <iomanip>

int main() {

	std::ifstream inputFile("input.txt", std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	int horizontalPosition = 0;
	int depth = 0;
	int value;
	std::string command;

	while (inputFile >> command >> value) {
		if (command.compare("forward") == 0) horizontalPosition += value;
		else if (command.compare("up") == 0) depth -= value;
		else if (command.compare("down") == 0) depth += value;
	}

	std::cout << "x = " << horizontalPosition 
		  << ", y = " << depth
		  << ", x * y = " << horizontalPosition * depth
		  << std::endl;

	return 0;
}
