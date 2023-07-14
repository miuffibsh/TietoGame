#include "Map.h"

int main(int argc, char** argv) {
	std::string mapFile;

	// Check if command-line argument is provided
	if (argc > 1) {
		mapFile = argv[1];
	}
	else {
		// Check if "mapa.txt" file exists
		std::ifstream file("mapa.txt");
		if (file.good()) {
			mapFile = "mapa.txt";
		}
		else {
			std::cout << "No map file provided and 'mapa.txt' not found.\n";
			std::cin.ignore();
			return -1;
		}
		file.close();
	}

	// Read rows and columns from the map file
	std::ifstream inputFile(mapFile);
	if (inputFile.is_open()) {
		std::string line;
		int rows = 0;
		int columns = 0;
		while (std::getline(inputFile, line)) {
			if (line.find("wiersze = ") != std::string::npos) {
				rows = std::stoi(line.substr(line.find("wiersze = ") + 10));    // Skip first 10 characters
			}
			else if (line.find("kolumny = ") != std::string::npos) {
				columns = std::stoi(line.substr(line.find("kolumny = ") + 10));    // Skip first 10 characters
			}
		}
		inputFile.close();

		// Generate and print the map
		Map generator(rows, columns);
		generator.generateRandomMap();
		generator.printMap(mapFile);
	}
	else {
		std::cout << "Failed to open " << mapFile << " for reading.\n";
		std::cin.ignore();
		return -1;
	}

	std::cin.ignore();
	return 0;
}
