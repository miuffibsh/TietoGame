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

int main(int argc, char* argv[]) {
	std::string programName;
	std::string mapFile;
	std::string statusFile;
	std::string ordersFile;
	double timeLimit;
	std::string programNameE;
	double timeLimitE;

	if (argc == 6) {
		programName = argv[1];
		mapFile = argv[2];
		statusFile = argv[3];
		ordersFile = argv[4];
		timeLimit = std::stod(argv[5]);
	}
	else {
		std::cout << "Welcome to Tietogra!\nAfter the game, the result, map and orders will be saved to the file rezultaty.txt.\n";
		std::cout << "Format: ./<name of the player program> mapa.txt status.txt rozkazy.txt [time limit in seconds]\n";
		std::cout << "Input for the player: ";
		std::cin >> programName >> mapFile >> statusFile >> ordersFile >> timeLimit;

		// Validate if the input starts with "./"
		if (programName.substr(0, 2) != "./") {
			std::cout << "Invalid program name. Program name should start with './'\n";
			return 1;
		}
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

	std::cout << "Do you want to generate a map? [y/n]\n";
	char generateMapChoice;
	std::cin >> generateMapChoice;
	std::system("clear");
	if (generateMapChoice == 'y' || generateMapChoice == 'Y') {
		std::string command = "./Tietogra " + mapFile;
		std::system(command.c_str());
		std::cout << "The generated map was saved to the map file.\n";
	}

	// Loading mapa.txt
	std::vector<std::vector<char>> map = loadMap(mapFile);

	Mediator mediator;
	mediator.startGame(programName, programNameE, 2000, timeLimit, timeLimitE, map, statusFile, ordersFile, mapFile);

	std::cin.ignore();
	std::cin.get();
	return 0;
}
