#include <iostream>

void calcMultiply(uint64_t firstPlayerStartingSpace, uint64_t secondPlayerStartingSpace) {

	bool player1Win = true;
	uint64_t rolls = 0;
	uint64_t player1Score = 0;
	uint64_t player2Score = 0;
	uint64_t player1Space = firstPlayerStartingSpace;
	uint64_t player2Space = secondPlayerStartingSpace;
	uint64_t rollingScore = 0;
	uint64_t dice = 0;

	while (1) {
		
		for (uint32_t i = 0; i < 3; i++) {
			dice = dice + 1 > 100 ? 1: dice + 1;
			rollingScore += dice;
		}
		

		rolls += 3;
		player1Space = (player1Space - 1 + rollingScore) % 10 + 1;
		player1Score += player1Space;
		std::cout << "rollingScore = " << rollingScore
			<< ", player1Space = " << player1Space
			<< ", player1Score = " << player1Score << std::endl;
		if (player1Score >= 1000) {
			player1Win = true;
			break;
		}

		rollingScore = 0;

		for (uint32_t i = 0; i < 3; i++) {
			dice = dice + 1 > 100 ? 1: dice + 1;
			rollingScore += dice;
		}

		rolls += 3;
		player2Space = (player2Space - 1 + rollingScore) % 10 + 1;
		player2Score += player2Space;
		std::cout << "rollingScore = " << rollingScore
			<< ", player2Space = " << player2Space
			<< ", player2Score = " << player2Score << std::endl;

		if (player2Score >= 1000) {
			player1Win = false;
			break;
		}

		rollingScore = 0;
	}

	if (player1Win) {
		std::cout << "player 1 wins, " << player2Score * rolls << std::endl;
	}
	else {
		std::cout << "player 2 wins, " << player1Score * rolls << std::endl;
	}

	std::cout << "rolls = " << rolls << std::endl;
}


int main() {

	calcMultiply(10, 2);

	return 0;
}
