#include "Mediator.h"

std::vector<std::vector<char>> loadMap(const std::string& mapFile)
{
	std::vector<std::vector<char>> map;

	std::ifstream file(mapFile);
	if (!file.is_open()) {
		// Handle file opening error
		return map;
	}

	std::string line;
	bool storeMap = false;  // Flag to determine if map was generated

	while (std::getline(file, line)) {
		if (line.find("Wygenerowana mapa:") != std::string::npos) {
			storeMap = true;
			continue;
		}

		if (storeMap) {
			std::vector<char> row;
			for (char ch : line) {
				row.push_back(ch);
			}
			map.push_back(row);
		}
	}

	file.close();
	return map;
}

int main() {
	std::string programName;
	std::string mapFile;
	std::string statusFile;
	std::string ordersFile;
	double timeLimit;
	std::string programNameE;
	double timeLimitE;

	std::cout << "Welcome to Tietogra. Do you want to generate a map? [y/n]\n";
	char generateMapChoice;
	std::cin >> generateMapChoice;
	std::system("clear");
	std::cout << "The generated map was saved to the file mapa.txt\n";
	if (generateMapChoice == 'y' || generateMapChoice == 'Y') {
		system("Tietogra.exe");
	}

	std::cout << "Format: ./<name of the player program> mapa.txt status.txt rozkazy.txt [time limit in seconds]\n";
	std::cout << "Input for the player: ";
	std::cin >> programName >> mapFile >> statusFile >> ordersFile >> timeLimit;

	// Validate if the input starts with "./"
	if (programName.substr(0, 2) != "./") {
		std::cout << "Invalid program name. Program name should start with './'\n";
		return 1;
	}

	std::cout << "Format: ./<name of the player program> [time limit in seconds]\n";
	std::cout << "Input for the enemy: ";
	std::cin >> programNameE >> timeLimitE;

	// Validate if the input starts with "./"
	if (programNameE.substr(0, 2) != "./") {
		std::cout << "Invalid program name. Program name should start with './'\n";
		return 1;
	}

	// Remove the "./" from the program name
	programName = programName.substr(2);
	programNameE = programNameE.substr(2);

	// Loading mapa.txt
	std::vector<std::vector<char>> map = loadMap(mapFile);

	Mediator mediator;
	mediator.startGame(programName, programNameE, 2000, timeLimit, timeLimitE, map, statusFile, ordersFile);

	std::cin.ignore();
	std::cin.get();
	return 0;
}
