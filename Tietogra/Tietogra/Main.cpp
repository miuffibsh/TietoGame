#include <iostream>
#include <fstream>
#include <string>
#include "Map.h"

int main() {
	// Read rows and columns from mapa.txt
	std::ifstream inputFile("mapa.txt");
	if (inputFile.is_open()) {
		std::string line;
		int rows = 0;
		int columns = 0;
		while (std::getline(inputFile, line)) {
			if (line.find("wiersze = ") != std::string::npos) {
				rows = std::stoi(line.substr(line.find("wiersze = ") + 10));	// Skip first 10 characters
			}
			else if (line.find("kolumny = ") != std::string::npos) {
				columns = std::stoi(line.substr(line.find("kolumny = ") + 10));	// Skip first 10 characters
			}
		}
		inputFile.close();

		// Generate and print the map
		Map generator(rows, columns);
		generator.generateRandomMap();
		generator.printMap("mapa.txt");
	}
	else {
		std::cout << "Failed to open mapa.txt for reading." << std::endl;
		std::cin.ignore();
		return -1;
	}



	std::cin.ignore();
	return 0;
}
