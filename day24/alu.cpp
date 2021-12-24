#include <iostream>
#include <vector>

uint32_t pow10(int n) {
	uint32_t res = 1;
	while (n > 0) {
		res *= 10;
		n--;
	}

	return res;
}

int extract(uint32_t number, int pos) {
	return (number / pow10(pos)) % 10;
}

bool findMONADNumber(uint32_t input, std::string &monadNum) {
	std::vector<int> plus{15, 12, 15, -1, -1, 2, -1, -1, 10, -1, 0, 0 , -1, -1};
	std::vector<int> checkpoint{-1, -1, -1, 9, 7, -1, 1, 16, -1, 15, -1, -1, 4, 0}; 
	std::vector<int> maxNum {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	int z = 0;
	std::string mNum;
	int digitIndex = 6;

	for (uint32_t i = 0; i < 14; i++) {

		if (checkpoint[i] == -1) {
			if (digitIndex < 0) {
				std::cout << "Something seriously go wrong!" << std::endl;
				exit(EXIT_FAILURE);
			}
			z = 26 * z + extract(input, digitIndex) + plus[i]; 
			//std::cout << "z = " << z << std::endl;
			digitIndex--;
		}
		else {
			maxNum[i] = (z % 26) - checkpoint[i];
			z = z / 26;
			//std::cout << "maxNum[ " << i << " ] = "  << maxNum[i] 
		//		<< ", z = " << z << std::endl;
			if (maxNum[i] <= 0 or maxNum[i] > 9) return false;
		}
	}

	if (z == 0) {
		mNum += std::to_string(extract(input, 6));
	        mNum += std::to_string(extract(input, 5)); 
	        mNum +=		std::to_string(extract(input, 4));
	        mNum +=		std::to_string(maxNum[3]);
		mNum +=	std::to_string(maxNum[4]);
		mNum +=	std::to_string(extract(input, 3)); 
		mNum +=	std::to_string(maxNum[6]); 
		mNum +=	std::to_string(maxNum[7]);
		mNum +=	std::to_string(extract(input, 2));
		mNum +=	std::to_string(maxNum[9]);
		mNum +=	std::to_string(extract(input, 1));
		mNum +=	std::to_string(extract(input, 0));
		mNum +=	std::to_string(maxNum[12]);
		mNum +=	std::to_string(maxNum[13]);
		std::cout << input << std::endl;
		monadNum = mNum;
		return true;
	}

	//std::cout << "z = " << z << std::endl;
	return false;
}

bool isValidMONADDigis(uint32_t number) {
	for (uint32_t i = 0; i < 7; i++) {
		if (extract(number, i) == 0) return false;
	}
	return true;
}

void findMaxMONADNumber(void) {
	uint32_t high = 9999999;
	uint32_t low = 1111111;
	std::string monadNum;

	for (uint32_t i = high; i >= low; i--) {
		//std::cout << "i = " << i << std::endl;
		if (isValidMONADDigis(i) and findMONADNumber(i, monadNum)) break; 
	}
	std::cout << monadNum << std::endl;
}

void findMinMONADNumber(void) {
	uint32_t high = 9999999;
	uint32_t low = 1111111;
	std::string monadNum;

	for (uint32_t i = low; i <= high; i++) {
		//std::cout << "i = " << i << std::endl;
		if (isValidMONADDigis(i) and findMONADNumber(i, monadNum)) break; 
	}
	std::cout << monadNum << std::endl;
}

int main() {

	findMaxMONADNumber();

	findMinMONADNumber();

	return 0;
}
