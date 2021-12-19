#include <iostream>

int height(int vy) {
	if (vy >= 0)
		return vy * (vy + 1) / 2;
	else 
		return -vy * (vy + 1) / 2;
}

std::pair<int, int> trickShort(std::pair<int, int> xRange, std::pair<int, int> yRange) {

	int x = 0, y = 0;
	int maxY = INT32_MIN;
	int vxI = 0, vyI = 0;
	int count = 0;
	for (int vy = yRange.first; vy <= abs(yRange.first);  vy++) {
		for (int vx = 0; vx <= xRange.second; vx++) {
			int vvx = vx;
			int vvy = vy;
			while (!(x > xRange.second or y < yRange.first)) {
				x += vvx;
				y += vvy;
				if (vvx > 0) vvx--;
				else if (vvx < 0) vvx++;
				vvy--;

				if (x <= xRange.second and x >= xRange.first and
				    y <= yRange.second and y >= yRange.first) {
					if (maxY < height(vy)) {
						vxI = vx;
						vyI = vy;
						maxY = height(vy);
					}
					count++;
					// to ensure one distinct solution
					break;
				}
			}
			//std::cout << "vx = " << vx << ", " << vy << std::endl;
			x = 0;
			y = 0;
		}
	}
	std::cout << "vxI = " << vxI << ", vyI = " << vyI << std::endl;
	return {maxY, count};
}

int main() {

	auto tS =  trickShort({81, 129}, {-150, -108});

	std::cout << "max height = " << tS.first << std::endl;

	std::cout << "num of initial values = " << tS.second << std::endl;

	return 0;
}

