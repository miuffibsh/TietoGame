#include "Mediator.h"

std::vector<std::tuple<char, int, int, int, int, int>> unitStats = {
	std::make_tuple('K', 70, 5, 400, 1, 5),
	std::make_tuple('S', 60, 2, 250, 1, 3),
	std::make_tuple('A', 40, 2, 250, 5, 3),
	std::make_tuple('P', 50, 2, 200, 2, 3),
	std::make_tuple('R', 90, 2, 500, 1, 4),
	std::make_tuple('C', 50, 2, 800, 7, 6),
	std::make_tuple('W', 20, 2, 100, 1, 2)
};

std::vector<char> damageType = { 'K', 'S', 'A', 'P', 'C', 'R', 'W', 'B' };

std::vector<std::vector<int>> damageValue = {
	{35, 35, 35, 35, 35, 50, 35, 35},
	{30, 30, 30, 20, 20, 30, 30, 30},
	{15, 15, 15, 15, 10, 10, 15, 15},
	{35, 15, 15, 15, 15, 10, 15, 10},
	{40, 40, 40, 40, 40, 40, 40, 50},
	{10, 10, 10, 10, 10, 10, 10, 50},
	{5, 5, 5, 5, 5, 5, 5, 1}
};

struct PlayerStatus {
	char affiliation;
	char type;
	int id;
	int xValue;
	int yValue;
	int enduranceLeft;
	char production;
};

std::vector<PlayerStatus> createStatusOfPlayers(int xValue1, int yValue1, int xValue2, int yValue2)
{
	std::vector<PlayerStatus> statusOfPlayers;

	PlayerStatus player1 = { 'P', 'B', 0, xValue1, yValue1, 200, 0 };
	PlayerStatus player2 = { 'E', 'B', 1, xValue2, yValue2, 200, 0 };

	statusOfPlayers.push_back(player1);
	statusOfPlayers.push_back(player2);

	return statusOfPlayers;
}

void criticalError(const std::vector<std::tuple<std::string, int, std::string>>& allOrders, const std::string& message) {
	std::ofstream outputFile("rezultaty.txt", std::ios::app);  // Open the file in append mode

	if (!outputFile.is_open()) {
		std::cout << "Failed to open rezultaty.txt for writing.\n";
		return;
	}

	// Write all the orders to the file
	for (const auto& order : allOrders) {
		outputFile << std::get<0>(order) << '\n';
	}

	// Add "Critical Error" at the end of the file
	std::string errorMessage = message;
	outputFile << errorMessage;

	outputFile.close();

	// Terminate the program
	std::exit(1);
}

void searchForErrors(const std::vector<PlayerStatus>& statusOfPlayers, const std::vector<std::string>& currentOrders,
	const std::vector<std::string>& currentOrdersE, long gold, long goldE,
	const std::vector<std::vector<char>>& map, const std::vector<std::tuple<std::string, int, std::string>>& allOrders) {
	// Check if the ID of the unit from currentOrders and currentOrdersE exists in statusOfPlayers
	for (const std::string& order : currentOrders) {
		std::stringstream ss(order);
		int unitID;
		char type;
		ss >> unitID >> type;

		bool foundID = false;
		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.id == unitID) {
				foundID = true;
				break;
			}
		}

		if (!foundID) {
			criticalError(allOrders, "Unit ID not found in statusOfPlayers");
		}
	}

	for (const std::string& order : currentOrdersE) {
		std::stringstream ss(order);
		int unitID;
		char type;
		ss >> unitID >> type;

		bool foundID = false;
		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.id == unitID) {
				foundID = true;
				break;
			}
		}

		if (!foundID) {
			criticalError(allOrders, "Unit ID not found in statusOfPlayers");
		}
	}

	// Check for attacking unit of the same affiliation
	for (const std::string& order : currentOrders) {
		std::stringstream ss(order);
		int unitID;
		char type;
		int targetID;
		ss >> unitID >> type >> targetID;

		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.id == unitID && playerStatus.affiliation == 'P') {
				for (const PlayerStatus& targetStatus : statusOfPlayers) {
					if (targetStatus.id == targetID && targetStatus.affiliation == 'P') {
						criticalError(allOrders, "Attacking unit of the same affiliation");
					}
				}
			}
		}
	}

	for (const std::string& order : currentOrdersE) {
		std::stringstream ss(order);
		int unitID;
		char type;
		int targetID;
		ss >> unitID >> type >> targetID;

		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.id == unitID && playerStatus.affiliation == 'E') {
				for (const PlayerStatus& targetStatus : statusOfPlayers) {
					if (targetStatus.id == targetID && targetStatus.affiliation == 'E') {
						criticalError(allOrders, "Attacking unit of the same affiliation");
					}
				}
			}
		}
	}

	// Check for order to build a unit while the base is occupied with production
	for (const std::string& order : currentOrders) {
		std::stringstream ss(order);
		int unitID;
		char type;
		std::string productionType;
		ss >> unitID >> type >> productionType;

		if (type == 'B' && productionType != "0") {
			for (const PlayerStatus& playerStatus : statusOfPlayers) {
				if (playerStatus.id == unitID && playerStatus.production != '0') {
					criticalError(allOrders, "Base already occupied with production");
				}
			}
		}
	}

	for (const std::string& order : currentOrdersE) {
		std::stringstream ss(order);
		int unitID;
		char type;
		std::string productionType;
		ss >> unitID >> type >> productionType;

		if (type == 'B' && productionType != "0") {
			for (const PlayerStatus& playerStatus : statusOfPlayers) {
				if (playerStatus.id == unitID && playerStatus.production != '0') {
					criticalError(allOrders, "Base already occupied with production");
				}
			}
		}
	}
}


void readOrders(const std::string& orderFile, std::vector<std::string>& currentOrders, std::vector<std::string>& currentOrdersE,
	std::vector<std::tuple<std::string, int, std::string>>& allOrders, int turn) {
	std::ifstream file(orderFile);
	if (!file.is_open()) {
		std::cout << "Failed to open " << orderFile << " for reading.\n";
		return;
	}

	std::string line;
	std::string playerName;
	while (std::getline(file, line)) {
		// Store the order in the appropriate vector
		if (line.find('B') != std::string::npos || line.find('A') != std::string::npos || line.find('M') != std::string::npos){
			std::stringstream ss(line);
			int index;
			char type;
			std::string value1;
			std::string value2;
			std::string value3;

			if (turn % 2 == 1) {
				playerName = "Player";
			}
			else if (turn % 2 == 0) {
				playerName = "Enemy";
			}

			ss >> index >> type;
			if (type == 'B') {
				ss >> value1;
				currentOrders.push_back(line);
			}
			else if (type == 'A') {
				ss >> value1 >> value2;
				currentOrdersE.push_back(line);
			}
			else if (type == 'M') {
				ss >> value1 >> value2 >> value3;
			}

			// Store the order in allOrders with additional information (playerName and turn)
			allOrders.emplace_back(line, turn, playerName);
		}
	}

	file.close();

	// Clear the order file
	std::ofstream clearFile(orderFile, std::ofstream::out | std::ofstream::trunc);
	if (clearFile.is_open()) {
		clearFile.close();
	}
	else {
		std::cout << "Failed to clear " << orderFile << ".\n";
	}
}

void updateState(const std::vector<std::tuple<char, int, int, int, int, int>>& unitStats,
	const std::vector<char>& damageType,
	const std::vector<std::vector<int>>& damageValue,
	const std::string& statusFile,
	const std::vector<std::string>& currentOrders,
	const std::vector<std::string>& currentOrdersE,
	const std::vector<std::vector<char>>& map,
	int turn, long gold, long goldE) {

	// Read the status file
	std::ifstream statusTxt(statusFile);
	if (!statusTxt.is_open()) {
		std::cout << "Failed to open " << statusFile << " for reading.\n";
		return;
	}

	// Read the status data
	std::vector<PlayerStatus> statusOfPlayers;
	std::string line;
	while (std::getline(statusTxt, line)) {
		std::istringstream iss(line);
		PlayerStatus playerStatus;
		iss >> playerStatus.affiliation >> playerStatus.type >> playerStatus.id >> playerStatus.xValue
			>> playerStatus.yValue >> playerStatus.enduranceLeft >> playerStatus.production;
		statusOfPlayers.push_back(playerStatus);
	}

	statusTxt.close();

	// Process current orders
	for (const auto& order : currentOrders) {
		std::istringstream iss(order);
		int unitID;
		char orderType;
		std::string typeOfUnit;

		iss >> unitID >> orderType >> typeOfUnit;

		// Check if the order is to produce a unit
		if (orderType == 'B') {
			// Check if the base is player base
			if (map[statusOfPlayers[unitID].xValue][statusOfPlayers[unitID].yValue] == '1') {
				// Find the unit in the unitStats based on the typeOfUnit
				auto unitIter = std::find_if(unitStats.begin(), unitStats.end(), [&](const auto& unit) {
					return std::get<0>(unit) == typeOfUnit[0];
				});

				if (unitIter != unitStats.end()) {
					// Extract the required information from unitStats
					char unitType = std::get<0>(*unitIter);
					int enduranceLeft = std::get<1>(*unitIter);
					int goldCost = std::get<3>(*unitIter);
					int turnsToProduce = std::get<5>(*unitIter);

					// Check if the base is not already producing a unit
					if (statusOfPlayers[unitID].production == 0) {
						// Subtract the gold cost from player's gold
						gold -= goldCost;

						// Update the base's production information
						statusOfPlayers[unitID].production = turnsToProduce;

						// Write the unit information to the status file
						std::ofstream updatedStatusFile(statusFile, std::ios::app);
						if (!updatedStatusFile.is_open()) {
							std::cout << "Failed to open " << statusFile << " for writing.\n";
							return;
						}

						updatedStatusFile << "P " << unitType << ' ' << unitID << ' '
							<< statusOfPlayers[unitID].xValue << ' ' << statusOfPlayers[unitID].yValue
							<< ' ' << enduranceLeft << ' ' << statusOfPlayers[unitID].production << '\n';

						updatedStatusFile.close();
					}
				}
			}
		}
	}
}



void swapAffiliations(std::vector<PlayerStatus>& statusOfPlayers, const std::string& statusFile, long gold) {
	for (PlayerStatus& playerStatus : statusOfPlayers) {
		if (playerStatus.affiliation == 'P') {
			playerStatus.affiliation = 'E';
		}
		else if (playerStatus.affiliation == 'E') {
			playerStatus.affiliation = 'P';
		}
	}

	std::ofstream file(statusFile);
	if (!file.is_open()) {
		std::cout << "Failed to open " << statusFile << " for writing.\n";
		return;
	}

	// Write gold value to the first line
	file << gold << '\n';

	// Write updated status of players to the file
	for (const PlayerStatus& playerStatus : statusOfPlayers) {
		file << playerStatus.affiliation << ' ' << playerStatus.type << ' '
			<< playerStatus.id << ' ' << playerStatus.xValue << ' '
			<< playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
			<< playerStatus.production << '\n';
	}

	file.close();
}

Mediator::Mediator() {
	
};

void Mediator::startGame(const std::string& programName, const std::string& programNameE, int maxGameTurns,
	double maxTimeLimit, double maxTimeLimitE, std::vector<std::vector<char>> map, const std::string& statusFile,
	const std::string& ordersFile) {
	long gold = 2000;
	long goldE = 2000;
	std::vector<std::string> currentOrders;
	std::vector<std::string> currentOrdersE;
	std::vector<std::tuple<std::string, int, std::string>> allOrders;

	/*// Open the status file and clear its content
	std::ofstream statusTxt(statusFile);
	if (!statusTxt.is_open()) {
		// Handle file opening error
		return;
	}*/

	// Write gold value to the status file
	//statusTxt << gold << "\n";

	// Find the positions of '1' and '2' on the map
	int xValue1 = -1;
	int yValue1 = -1;
	int xValue2 = -1;
	int yValue2 = -1;
	for (int i = 0; i < map.size(); ++i) {
		for (int j = 0; j < map[i].size(); ++j) {
			if (map[i][j] == '1') {
				xValue1 = j;
				yValue1 = i;
			}
			else if (map[i][j] == '2') {
				xValue2 = j;
				yValue2 = i;
			}
		}
	}

	// Check if '1' was found on the map
	if (xValue1 == -1 || yValue1 == -1) {
		std::cout << "Error: '1' not found on the map.\n";
		return;
	}

	// Check if '2' was found on the map
	if (xValue2 == -1 || yValue2 == -1) {
		std::cout << "Error: '2' not found on the map.\n";
		return;
	}

	// Write the initial player base statuses to the status file
	std::vector<PlayerStatus> statusOfPlayers = createStatusOfPlayers(xValue1, yValue1, xValue2, yValue2);
	/*
	for (const PlayerStatus& playerStatus : statusOfPlayers) {
		statusTxt << playerStatus.affiliation << ' ' << playerStatus.type << ' '
			<< playerStatus.id << ' ' << playerStatus.xValue << ' '
			<< playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
			<< playerStatus.production << '\n';
	}

	statusTxt.close();
	*/
	for (int turn = 1; turn <= maxGameTurns; turn++) {
		std::cout << "Turn " << turn << "\n";

		// Run programName on odd iterations
		if (turn % 2 == 1) {
			// Start timer
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

			std::system(programName.c_str());

			// Stop timer and calculate elapsed time
			std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedSeconds = endTime - startTime;
			std::cout << "Elapsed seconds: " << elapsedSeconds.count() << std::endl;

			// Check if elapsed time exceeds the time limit
			if (elapsedSeconds.count() > (maxTimeLimit != 0 ? maxTimeLimit : 5)) {
				std::cout << "Time limit player 1\n";
				break; // Break the loop if time limit is exceeded
			}

			// Read player orders
			//readOrders(ordersFile, currentOrders, currentOrdersE, allOrders, turn);

			// Search for critical errors
			//searchForErrors(statusOfPlayers, currentOrders, currentOrdersE, gold, goldE, map, allOrders);

			// Update current state
			//updateState(unitStats, damageType, damageValue, statusFile, currentOrders, currentOrdersE, map, turn, gold, goldE);

			// Swaps affiliations
			//swapAffiliations(statusOfPlayers, statusFile, gold);
		}

		// Run programNameE on even iterations
		if (turn % 2 == 0) {
			// Start timer
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

			//std::system(programNameE.c_str());

			// Stop timer and calculate elapsed time
			std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedSeconds = endTime - startTime;

			// Check if elapsed time exceeds the time limit
			if (elapsedSeconds.count() > (maxTimeLimitE != 0 ? maxTimeLimitE : 5)) {
				std::cout << "Time limit player 2\n";
				break; // Break the loop if time limit is exceeded
			}

			// Read enemy orders
			//readOrders(ordersFile, currentOrders, currentOrdersE, allOrders, turn);

			// Search for critical errors
			//searchForErrors(statusOfPlayers, currentOrders, currentOrdersE, gold, goldE, map, allOrders);

			// Update current state
			//updateState(unitStats, damageType, damageValue, statusFile, currentOrders, currentOrdersE, map, turn, gold, goldE);

			// Swaps affiliations
			//swapAffiliations(statusOfPlayers, statusFile, goldE);
		}
	}
};