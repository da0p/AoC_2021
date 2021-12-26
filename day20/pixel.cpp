#include <iostream>
#include <fstream>
#include <vector>

uint32_t isEmptyLine(std::string line) {
	return line.length() == 0;
}

std::pair<std::vector<uint32_t>, std::vector<std::vector<uint32_t>>> 
parseInput(std::string inputFileName) {

	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "Could not open file" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string line;
	std::vector<uint32_t> lookUp;
	std::vector<uint32_t> inputImageRow;
	std::vector<std::vector<uint32_t>> inputImage;

	do {
		std::getline(inputFile, line);
		for (auto c : line) {
			if (c == '.') lookUp.push_back(0);
			else if (c == '#') lookUp.push_back(1);
		}
	} while (!isEmptyLine(line));


	while (!inputFile.eof()) {
		std::getline(inputFile, line);
		if (!isEmptyLine(line)) {
			for (auto c : line) {
				if (c == '.') inputImageRow.push_back(0);
				else if (c == '#') inputImageRow.push_back(1);
			}
			inputImage.push_back(inputImageRow);
			inputImageRow.clear();
		}
	}

	return {lookUp, inputImage};
}

void displayImage(std::vector<std::vector<uint32_t>> image) {

	std::cout << "---------------------------" << std::endl;
	for (uint32_t r = 0; r < image.size(); r++) {
		for (uint32_t c = 0; c < image[0].size(); c++) {
			if (image[r][c]) std::cout << "#";
			else std::cout << ".";
		}
		std::cout << std::endl;
	}
}

uint32_t countLitPixels(std::vector<std::vector<uint32_t>> outputImage,
		uint32_t offsetRow, uint32_t offsetCol) {
	uint32_t sum = 0;

	for (uint32_t i = offsetRow; i < outputImage.size() - offsetRow; i++) {
		for (uint32_t j = offsetCol; j < outputImage[0].size() - offsetCol; j++) {
			sum += outputImage[i][j];
		}
	}

	return sum;
}

std::vector<std::vector<uint32_t>> paddingImage(std::vector<std::vector<uint32_t>> inputImage, uint32_t padding, uint32_t defaultValue) {

	uint32_t inputImageRow = inputImage.size();
	uint32_t inputImageCol = inputImage[0].size();

	std::vector<std::vector<uint32_t>> paddedImage (inputImageRow + padding * 2, std::vector<uint32_t>(inputImageCol + padding * 2));

	for (uint32_t i = 0; i < inputImageRow + padding * 2; i++) {
		for (uint32_t j = 0; j < inputImageCol + padding * 2; j++) {
			if (i < padding or i >= inputImageRow + padding) paddedImage[i][j] = defaultValue;
			else if (j < padding or j >= inputImageCol + padding) paddedImage[i][j] = defaultValue;
			else {
				paddedImage[i][j] = inputImage[i - padding][j - padding];
			}
		}
	}

	return paddedImage;
}

std::vector<std::vector<uint32_t>> enhanceImage(std::vector<std::vector<uint32_t>> paddedImage, uint32_t defaultValue, std::vector<uint32_t> lookUp) {

		uint32_t paddedImageRow = paddedImage.size();
		uint32_t paddedImageCol = paddedImage[0].size();
		std::vector<std::vector<uint32_t>> outputImage (paddedImageRow, std::vector<uint32_t>(paddedImageCol, 0)); 

	for (uint32_t i = 0; i < paddedImageRow; i++) {
		for (uint32_t j = 0; j < paddedImageCol; j++) {
			if (i == 0 or i == paddedImageRow - 1) outputImage[i][j] = defaultValue;
			else if (j == 0 or j == paddedImageCol - 1) outputImage[i][j] = defaultValue;
			else {
				uint32_t lookUpIndex = 
					paddedImage[i + 1][j + 1] + paddedImage[i + 1][j] * 2 +
					paddedImage[i + 1][j - 1] * 4 + paddedImage[i][j + 1] * 8 +
					paddedImage[i][j] * 16 + paddedImage[i][j - 1] * 32 +
					paddedImage[i - 1][j + 1] * 64 + paddedImage[i - 1][j] * 128 + 
					paddedImage[i - 1][j - 1] * 256;
				outputImage[i][j] = lookUp[lookUpIndex];
				}
			}
	}

	return outputImage;
}


std::vector<std::vector<uint32_t>> filterImage(std::vector<std::vector<uint32_t>> inputImage, std::vector<uint32_t> lookUp, uint32_t steps) {

	std::vector<std::vector<uint32_t>> paddedImage = inputImage;
	
	for (uint32_t step = 0; step < steps; step++) {

		uint32_t defaultValue = step % 2;
		paddedImage = paddingImage(paddedImage, 10, defaultValue);
		paddedImage = enhanceImage(paddedImage, !defaultValue, lookUp);
	}
	return paddedImage;
}

void displayLookUp(std::vector<uint32_t> lookUp) {
	for (auto e : lookUp) {
		if (e) std::cout << "#";
		else std::cout << ".";
	}
}

int main(void) {

	auto [lookUp, inputImage] = parseInput("input.txt");

	auto outputImage = filterImage(inputImage, lookUp, 2);

	displayImage(outputImage);

	std::cout << "Part 1: Lit Pixels = " << countLitPixels(outputImage, 0, 0) << std::endl;

	outputImage = filterImage(inputImage, lookUp, 50);

	std::cout << "Part 2: Lit Pixels = " << countLitPixels(outputImage, 0, 0) << std::endl;


	return 0;
}
