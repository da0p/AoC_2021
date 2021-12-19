#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <deque>
#include <variant>
#include <memory>

std::map<char, uint8_t> decoder {
	{'0',  0}, {'1', 1},
	{'2',  2}, {'3', 3},
	{'4',  4}, {'5', 5},
	{'6',  6}, {'7', 7},
	{'8',  8}, {'9', 9},
	{'A',  10}, {'B',11},
	{'C',  12}, {'D',13},
	{'E',  14}, {'F',15}
};

struct Packet {
	uint64_t version;
	uint64_t id;
	std::variant<uint64_t, std::vector<std::unique_ptr<Packet>>> content;

	uint64_t sumOfHeaderVersion() {
		uint64_t sum = version;
		if (std::holds_alternative<std::vector<std::unique_ptr<Packet>>>(content)) {
			for (auto &sPkt : std::get<std::vector<std::unique_ptr<Packet>>>(content)) {
				sum += sPkt->sumOfHeaderVersion();
			}
		}
		return sum;
	}

	uint64_t evaluate() {
		switch (id) {
			case 0: {
				uint64_t sum = 0;
				for (auto &sPkt : std::get<1>(content)) {
					sum += sPkt->evaluate();
					}
				return sum;
			}
			case 1: {
				uint64_t product = 1;
				for (auto &sPkt : std::get<1>(content)) {
					product *= sPkt->evaluate();
				}
				return product;
			}
			case 2: {
				uint64_t min = UINT64_MAX;
				for (auto &sPkt : std::get<1>(content)) {
					min = std::min(min, sPkt->evaluate());
				}
				return min;
			}
			case 3: {
				uint64_t max = 0;
				for (auto &sPkt : std::get<1>(content)) {
					max = std::max(max, sPkt->evaluate());
				}
				return max;
			}
			case 4: {
				return std::get<0>(content);
			}
			case 5: {
				auto pIt = std::move(std::get<1>(content));
				if (pIt.size() != 2) {
					std::cout << "Size must be equal to 2" << std::endl;
					exit(EXIT_FAILURE);
				}

				return pIt[0]->evaluate() > pIt[1]->evaluate() ? 1 : 0;
			}
			case 6: {
				auto pIt = std::move(std::get<1>(content));
				if (pIt.size() != 2) {
					std::cout << "Size must be equal to 2" << std::endl;
					exit(EXIT_FAILURE);
				}

				return pIt[0]->evaluate() < pIt[1]->evaluate() ? 1 : 0;
			}
			case 7: {
				auto pIt = std::move(std::get<1>(content));
				if (pIt.size() != 2) {
					std::cout << "Size must be equal to 2" << std::endl;
					exit(EXIT_FAILURE);
				}

				return pIt[0]->evaluate() == pIt[1]->evaluate() ? 1 : 0;
			}
			default: {
				std::cout << "Something wrong! Should not be here" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
	
};

struct PacketIt {
	std::unique_ptr<Packet> packet;
	std::vector<bool>::const_iterator after;
};

uint64_t readBits(std::vector<bool>::const_iterator begin,
		std::vector<bool>::const_iterator end) {
	uint64_t bits = 0;

	for (; begin != end; ++begin) {
		bits = bits << 1;
		if (*begin) bits |= 1;
	}

	return bits;
}

uint64_t parseVersion(std::vector<bool>::const_iterator begin, 
			std::vector<bool>::const_iterator end) {
	uint64_t mVersion;

	if (end - begin >= 3) {
		mVersion = readBits(begin, begin + 3);
	}
	else {
		std::cout << "Requires equal or more than 3 bits" << std::endl;
		exit(EXIT_FAILURE);
	}

	return mVersion;
}

uint64_t parseId(std::vector<bool>::const_iterator begin,
		std::vector<bool>::const_iterator end) {
	uint64_t mId;

	if (end - begin >= 3) {
		mId = readBits(begin, begin + 3);
	}
	else {
		std::cout << "Requires equal or more than 3 bits" << std::endl;
		exit(EXIT_FAILURE);
	}
	return mId;
}

std::pair<uint64_t, std::vector<bool>::const_iterator>  
parseLiteralMessage(std::vector<bool>::const_iterator begin, 
			std::vector<bool>::const_iterator end) {

	uint64_t mMsg = 0, notLastPacket = 1;
		while (notLastPacket) {
			if (end - begin < 5) {
				std::cout << "Need more than 5 bits" << std::endl;
				exit(EXIT_FAILURE);
			}
			notLastPacket = *(begin++);
			mMsg = mMsg << 4;
			mMsg |= readBits(begin, begin + 4);
			begin += 4;
		}
	return {mMsg, begin};
}

PacketIt  parse(std::vector<bool>::const_iterator begin, 
		std::vector<bool>::const_iterator end) {
	
	PacketIt pIt;
	pIt.packet = std::make_unique<Packet>();
	pIt.packet->version = parseVersion(begin, begin + 3);
	begin += 3;
	pIt.packet->id = parseId(begin, begin + 3);
	begin += 3;
	if (pIt.packet->id == 4) {
		auto [literalMsg, after] = parseLiteralMessage(begin, end);
		pIt.packet->content.emplace<uint64_t>(literalMsg);
		begin = after;
	}
	else {
		std::vector<std::unique_ptr<Packet>> subPackets;
		auto lengthTypeId = *(begin++);
		//std::cout << "end - begin = " << end - begin << std::endl;
		if (end - begin < 15) {
			std::cout << "Need equal or more than 15 bits" << std::endl;
			exit(EXIT_FAILURE);
		}
		
		
		if (lengthTypeId == 0) {
			auto subPacketLength = readBits(begin, begin + 15);
			begin += 15;
			auto endOfSubPackets = begin + subPacketLength;

			while (begin < endOfSubPackets) {
				auto subPacketIt = parse(begin, endOfSubPackets);
				subPackets.push_back(std::move(subPacketIt.packet));
				begin = subPacketIt.after;
			}
		}
			
		if (lengthTypeId == 1) {
			if (end - begin < 11) {
				std::cout << "Need equal or more than 11 bits" << std::endl;
				exit(EXIT_FAILURE);
			}
			auto numOfSubPackets = readBits(begin, begin + 11);
			begin += 11;
			for (uint64_t i = 0; i < numOfSubPackets; i++) {
				auto subPacketIt = parse(begin, end);
				subPackets.push_back(std::move(subPacketIt.packet));
				begin = subPacketIt.after;
			}
		}

		pIt.packet->content.emplace<std::vector<std::unique_ptr<Packet>>>(std::move(subPackets));
	}

	pIt.after = begin;

	return pIt;
}

std::vector<bool>  parseInput(std::string inputFileName) 
{
	std::ifstream inputFile(inputFileName, std::ios::in);

	if (!inputFile) {
		std::cerr << "File could not be opened" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string unusedString; 
	std::vector<bool> bits;

	inputFile >> unusedString;

	for (auto c : unusedString) {
		if (decoder.count(c) > 0) {
			auto b = decoder[c];

			bits.push_back((b & 8) != 0 ? 1 : 0);
			bits.push_back((b & 4) != 0 ? 1 : 0);
			bits.push_back((b & 2) != 0 ? 1 : 0);
			bits.push_back((b & 1) != 0 ? 1 : 0);
		}
	}

	return bits;
}

void display(std::vector<bool> encodedInput) {
	uint64_t i = 0;
	for (auto b : encodedInput) {
		std::cout << b;
		i++;
		if (i > 3) {
			i = 0;
			std::cout << std::endl;
		}
	}
}

int main() {

	auto encodedInput = parseInput("input.txt");

	auto pIt = parse(encodedInput.begin(), encodedInput.end());

	std::cout << "sum of header version = " << pIt.packet->sumOfHeaderVersion() << std::endl; 

	std::cout << "evaluation = " << pIt.packet->evaluate() << std::endl;

	//display(encodedInput);
	
	return 0;
}
