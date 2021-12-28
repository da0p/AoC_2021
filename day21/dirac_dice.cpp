#include <iostream>
#include <vector>
#include <map>

uint64_t rolls(void) {
	static uint64_t die = 0;
	die = die + 1 > 100 ? 1 : die + 1;

	return die;
}

uint64_t calcMultiply(uint64_t firstPlayerStartingSpace, uint64_t secondPlayerStartingSpace) {

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
		return player2Score * dice;
	}
	else {
		return player1Score * dice;
	}
}

std::pair<uint64_t, uint64_t>  win(uint64_t player1Space, uint64_t player2Space, 
		uint64_t player1Score, uint64_t player2Score, 
		std::map<std::vector<uint64_t>, std::pair<uint64_t, uint64_t>> &memoi) {

	if (player1Score >= 21) return  {1, 0};

	if (player2Score >= 21) return {0, 1};
	
	if (memoi.count({player1Space, player2Space, player1Score, player2Score}) > 0) {
		return memoi[{player1Space, player2Space, player1Score, player2Score}];
	}

	std::pair<uint64_t, uint64_t> winner {0, 0};
	for (uint64_t i = 1; i <= 3; i++) {
		for (uint64_t j = 1; j <= 3; j++) {
			for (uint64_t k = 1; k <= 3; k++) {
				uint64_t newPlayer1Space = (player1Space + i + j + k - 1) % 10 + 1;
				uint64_t newPlayer1Score = player1Score + newPlayer1Space;
				auto who = win(player2Space, newPlayer1Space, 
						player2Score, newPlayer1Score, memoi);
				
				winner.first += who.second;
				winner.second += who.first;
			}
		}
	}
	memoi.insert({{player1Space, player2Space, player1Score, player2Score}, winner});

	return winner;
}

int main() {

	std::cout << "Part 1: " << calcMultiply(10, 2) << std::endl;
	
	std::map<std::vector<uint64_t>, std::pair<uint64_t, uint64_t>> memoi;
	auto winner = win(10, 2, 0, 0, memoi);

	std::cout << "Part 2: " << std::max(winner.first, winner.second) << std::endl;

	return 0;
}
