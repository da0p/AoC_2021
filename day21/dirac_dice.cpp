#include <iostream>

uint64_t rolls(void) {
	static uint64_t die = 0;
	die = die + 1 > 100 ? 1 : die + 1;

	return die;
}

void calcMultiply(uint64_t firstPlayerStartingSpace, uint64_t secondPlayerStartingSpace) {

	bool player1Win = true;
	uint64_t player1Score = 0;
	uint64_t player2Score = 0;
	uint64_t player1Space = firstPlayerStartingSpace;
	uint64_t player2Space = secondPlayerStartingSpace;
	uint64_t rollingScore = 0;
	uint64_t dice = 0;

	while (1) {
		
		dice += 3;
		rollingScore = rolls() + rolls() + rolls();
		
		player1Space = (player1Space - 1 + rollingScore) % 10 + 1;
		player1Score += player1Space;
		if (player1Score >= 1000) {
			player1Win = true;
			break;
		}

		dice += 3;
		rollingScore = rolls() + rolls() + rolls();

		player2Space = (player2Space - 1 + rollingScore) % 10 + 1;
		player2Score += player2Space;

		if (player2Score >= 1000) {
			player1Win = false;
			break;
		}
	}

	if (player1Win) {
		std::cout << "player 1 wins, " << player2Score * dice << std::endl;
	}
	else {
		std::cout << "player 2 wins, " << player1Score * dice << std::endl;
	}
}


int main() {

	calcMultiply(10, 2);

	return 0;
}
