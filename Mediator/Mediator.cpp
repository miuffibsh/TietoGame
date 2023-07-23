#include "Mediator.h"

void playerWins(int turn, const std::vector<std::tuple<std::string, int, std::string>>& allOrders, const std::vector<std::vector<char>>& map,
	const std::vector<std::tuple<char, int, int, int, int, int>>& unitStats) {
	std::string playerName;
	if (turn % 2 == 1) {
		playerName = "Player";
	}
	else if (turn % 2 == 0) {
		playerName = "Enemy";
	}

	std::ofstream outputFile("rezultaty.txt", std::ofstream::trunc);
	if (!outputFile.is_open()) {
		std::cerr << "Failed to open rezultaty.txt for writing.\n";
		std::exit(1);  // Terminate the program with an error status code
	}

	// Write the player name and result to the file
	outputFile << playerName << " wins.\n";

	// Write the map to the file
	for (const std::vector<char>& row : map) {
		for (char cell : row) {
			outputFile << cell;
		}
		outputFile << "\n";
	}

	// Write the contents of allOrders to the file
	int initialTurn = -1; // Initialize initialTurn for the Player with a default value
	int initialTurnE = -1; // Initialize initialTurnE for the Enemy with a default value
	int turnsToBuild = -1; // Initialize turnsToBuild for the Player with a default value
	int turnsToBuildE = -1; // Initialize turnsToBuildE for the Enemy with a default value

	for (const auto& order : allOrders) {
		const std::string& orderString = std::get<0>(order);
		char type = orderString[2]; // Get the character at index 1 to check for 'B'
		char typeUnit = orderString[4]; // Get the character at index 2 to check the unit type

		if (type == 'B') {
			if (std::get<2>(order) == "Player" && initialTurn == -1) {
				initialTurn = std::get<1>(order); // Assign the value to initialTurn
				// Search for the specific unit type in the unitStats vector
				for (const auto& unit : unitStats) {
					if (std::get<0>(unit) == typeUnit) {
						turnsToBuild = std::get<5>(unit); // Get the sixth element of the tuple
						break; // Break the loop as we found the required unit type
					}
				}
				outputFile << orderString << ' ' << std::get<1>(order) << ' ' << std::get<2>(order) << '\n';
			}
			else if (std::get<2>(order) == "Enemy" && initialTurnE == -1) {
				initialTurnE = std::get<1>(order); // Assign the value to initialTurnE
				// Search for the specific unit type in the unitStats vector
				for (const auto& unit : unitStats) {
					if (std::get<0>(unit) == typeUnit) {
						turnsToBuildE = std::get<5>(unit); // Get the sixth element of the tuple
						break; // Break the loop as we found the required unit type
					}
				}
				outputFile << orderString << ' ' << std::get<1>(order) << ' ' << std::get<2>(order) << '\n';
			}

			//std::cout << (initialTurn + turnsToBuild - std::get<1>(order)) << std::endl;

			if (std::get<2>(order) == "Player" && initialTurn != -1 && (initialTurn + turnsToBuild - std::get<1>(order) <= 2)) {
				initialTurn = -1;
			}
			else if (std::get<2>(order) == "Enemy" && initialTurnE != -1 && (initialTurnE + turnsToBuildE - std::get<1>(order) <= 2)) {
				initialTurnE = -1;
			}
		}
		else {
			// Write other orders directly to the output file
			outputFile << orderString << ' ' << std::get<1>(order) << ' ' << std::get<2>(order) << '\n';
		}
	}

	outputFile.close();

	//std::cin.ignore();  // Ignore any characters entered by the user
	//std::cin.get();     // Wait for the user to press Enter

	std::exit(0);  // Terminate the program with a success status code
}

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

	PlayerStatus player1 = { 'P', 'B', 0, xValue1, yValue1, 200, '0' };
	PlayerStatus player2 = { 'E', 'B', 1, xValue2, yValue2, 200, '0' };
	PlayerStatus player3 = { 'P', 'S', 2, xValue1, yValue1, 60, ' ' };
	PlayerStatus player4 = { 'E', 'A', 3, xValue2, yValue2, 40, ' ' };
	PlayerStatus player5 = { 'P', 'K', 4, xValue1, yValue1, 70, ' ' };
	PlayerStatus player6 = { 'E', 'K', 5, xValue2, yValue2, 70, ' ' };
	PlayerStatus player7 = { 'P', 'C', 6, xValue1, yValue1, 50, ' ' };
	PlayerStatus player8 = { 'E', 'A', 7, xValue2, yValue2, 40, ' ' };
	PlayerStatus player9 = { 'P', 'S', 8, xValue1, yValue1, 60, ' ' };
	PlayerStatus player10 = { 'E', 'S', 8, xValue2, yValue2, 60, ' ' };

	statusOfPlayers.push_back(player1);
	statusOfPlayers.push_back(player2);
	statusOfPlayers.push_back(player3);
	statusOfPlayers.push_back(player4);
	statusOfPlayers.push_back(player5);
	statusOfPlayers.push_back(player6);
	statusOfPlayers.push_back(player7);
	statusOfPlayers.push_back(player8);
	statusOfPlayers.push_back(player9);
	statusOfPlayers.push_back(player10);

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


void readOrders(const std::string& orderFile, std::vector<std::string>& currentOrders,
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
			//std::string value3;

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
				ss >> value1;
				currentOrders.push_back(line);
			}
			else if (type == 'M') {
				ss >> value1 >> value2;
				currentOrders.push_back(line);
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
	const std::vector<std::vector<char>>& map,
	int turn, long& gold, 
	const std::vector<std::tuple<std::string, int, std::string>>& allOrders) {

	// Read the status file
	std::ifstream statusTxt(statusFile);
	if (!statusTxt.is_open()) {
		std::cout << "Failed to open " << statusFile << " for reading.\n";
		return;
	}

	// Ignore the first line
	std::string dummyLine;
	std::getline(statusTxt, dummyLine);

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
		int xValue;
		int yValue;
		int enemyID;
		std::string typeOfUnit;
		PlayerStatus activeUnit = { 'P', 'B', 0, 0, 0, 999, ' ' };

		iss >> unitID >> orderType;

		// Check if the order is to produce a unit
		if (orderType == 'B') {

			iss >> typeOfUnit;
			// Find the unit in the unitStats based on the typeOfUnit
			auto unitIter = std::find_if(unitStats.begin(), unitStats.end(), [&](const auto& unit) {
				return std::get<0>(unit) == typeOfUnit[0];
			});

			if (unitIter != unitStats.end()) {
				// Extract the required information from unitStats
				char unitType = std::get<0>(*unitIter);
				int goldCost = std::get<3>(*unitIter);

				// Check if the base is not already producing a unit
				if (statusOfPlayers[unitID].production == '0') {
					// Subtract the gold cost from player's gold
					if (gold - goldCost >= 0) {
						gold -= goldCost;

						// Open the status file for writing
						std::ofstream updatedStatusFile(statusFile);
						if (!updatedStatusFile.is_open()) {
							std::cout << "Failed to open " << statusFile << " for writing.\n";
							return;
						}

						// Update the gold value in the first line
						updatedStatusFile << gold << '\n';

						// Find the player base with the specified unitID
						auto playerBaseIter = std::find_if(statusOfPlayers.begin(), statusOfPlayers.end(), [&](const PlayerStatus& playerStatus) {
							return playerStatus.affiliation == 'P' && playerStatus.id == unitID;
						});

						if (playerBaseIter != statusOfPlayers.end()) {
							// Update the line describing the player base unit
							*playerBaseIter = { 'P', 'B', unitID, playerBaseIter->xValue, playerBaseIter->yValue, playerBaseIter->enduranceLeft, unitType };

							// Write updated informations to status file
							for (const PlayerStatus& playerStatus : statusOfPlayers) {
								updatedStatusFile << playerStatus.affiliation << ' ' << playerStatus.type << ' '
									<< playerStatus.id << ' ' << playerStatus.xValue << ' '
									<< playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
									<< playerStatus.production << '\n';
							}
						}

						updatedStatusFile.close();

					}
				}
			}
		}

		// Check if the order is to move a unit
		if (orderType == 'M') {
			iss >> xValue >> yValue;

			// Find the player unit with the specified unitID
			auto playerUnitIter = std::find_if(statusOfPlayers.begin(), statusOfPlayers.end(), [&](const PlayerStatus& playerStatus) {
				return playerStatus.affiliation == 'P' && playerStatus.id == unitID;
			});

			if (playerUnitIter != statusOfPlayers.end()) {
				// Update the player unit's xValue and yValue
				playerUnitIter->xValue = xValue;
				playerUnitIter->yValue = yValue;

				// Open the status file for writing
				std::ofstream updatedStatusFile(statusFile);
				if (!updatedStatusFile.is_open()) {
					std::cout << "Failed to open " << statusFile << " for writing.\n";
					return;
				}

				// Update the gold value in the first line
				updatedStatusFile << gold << '\n';

				// Write the updated status of players to the file
				for (const PlayerStatus& playerStatus : statusOfPlayers) {
					updatedStatusFile << playerStatus.affiliation << ' ' << playerStatus.type << ' '
						<< playerStatus.id << ' ' << playerStatus.xValue << ' '
						<< playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
						<< playerStatus.production << '\n';
				}

				updatedStatusFile.close();

			}
		}

		// Check if the order is to attack a unit
		if (orderType == 'A') {
			iss >> enemyID;

			// Find the player unit with the specified unitID
			auto playerUnitIter = std::find_if(statusOfPlayers.begin(), statusOfPlayers.end(), [&](const PlayerStatus& playerStatus) {
				return playerStatus.affiliation == 'P' && playerStatus.id == unitID;
			});

			// Find the enemy unit with the specified enemyID
			auto enemyUnitIter = std::find_if(statusOfPlayers.begin(), statusOfPlayers.end(), [&](const PlayerStatus& playerStatus) {
				return playerStatus.affiliation == 'E' && playerStatus.id == enemyID;
			});

			if (playerUnitIter != statusOfPlayers.end() && enemyUnitIter != statusOfPlayers.end()) {
				// Get the index of player unit type in damageType vector
				auto playerUnitTypeIndex = std::find(damageType.begin(), damageType.end(), playerUnitIter->type) - damageType.begin();
				// Get the index of enemy unit type in damageType vector
				auto enemyUnitTypeIndex = std::find(damageType.begin(), damageType.end(), enemyUnitIter->type) - damageType.begin();

				// Check if the unit types are valid
				if (playerUnitTypeIndex < damageValue.size() && enemyUnitTypeIndex < damageValue[playerUnitTypeIndex].size()) {
					// Get the damage value from damageValue vector
					int damage = damageValue[playerUnitTypeIndex][enemyUnitTypeIndex];

					// Subtract the damage value from enemy unit's enduranceLeft
					enemyUnitIter->enduranceLeft -= damage;

					// Check if the enemy base is destroyed (unit with type == 'B')
					if (enemyUnitIter->type == 'B' && enemyUnitIter->enduranceLeft <= 0) {
						// Run the function indicating that the player wins
						playerWins(turn, allOrders, map, unitStats);
						return;
					}

					// Remove the enemy unit if its enduranceLeft is equal or less than 0
					if (enemyUnitIter->enduranceLeft <= 0) {
						statusOfPlayers.erase(enemyUnitIter);
					}

					// Open the status file for writing
					std::ofstream updatedStatusFile(statusFile);
					if (!updatedStatusFile.is_open()) {
						std::cout << "Failed to open " << statusFile << " for writing.\n";
						return;
					}

					// Update the gold value in the first line
					updatedStatusFile << gold << '\n';

					// Write the updated status of players to the file
					for (const PlayerStatus& playerStatus : statusOfPlayers) {
						updatedStatusFile << playerStatus.affiliation << ' ' << playerStatus.type << ' '
							<< playerStatus.id << ' ' << playerStatus.xValue << ' '
							<< playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
							<< playerStatus.production << '\n';
					}

					updatedStatusFile.close();
				}
			}
		}
	}
}

void createNewUnit(char unitType, int xValue, int yValue, const std::vector<std::tuple<char, int, int, int, int, int>>& unitStats, const std::string& statusFile, int turn) {
	// Read the contents of statusFile
	std::ifstream statusTxt(statusFile);
	if (!statusTxt.is_open()) {
		std::cout << "Failed to open " << statusFile << " for reading.\n";
		return;
	}

	// Ignore the first line
	std::string dummyLine;
	std::getline(statusTxt, dummyLine);

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

	// Find the base unit with matching type, xValue, and yValue
	char newUnitAffiliation = ' ';
	int newUnitEndurance = 0;
	int newUnitID = 0;
	int baseIDTemp = 0;

	for (const PlayerStatus& playerStatus : statusOfPlayers) {
		if (playerStatus.type == 'B' && playerStatus.xValue == xValue && playerStatus.yValue == yValue) {
			baseIDTemp = playerStatus.id;
			break;
		}
	}

	if (turn % 2 == 1 && baseIDTemp == 0) {
		newUnitAffiliation = 'P';
	}
	else if (turn % 2 == 0 && baseIDTemp == 0) {
		newUnitAffiliation = 'E';
	}
	else if (turn % 2 == 1 && baseIDTemp != 0) {
		newUnitAffiliation = 'E';
	}
	else if (turn % 2 == 0 && baseIDTemp != 0) {
		newUnitAffiliation = 'P';
	}

	// Find the lowest available ID
	for (int id = 0; id <= 1000; ++id) {
		bool idExists = false;
		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.id == id) {
				idExists = true;
				break;
			}
		}
		if (!idExists) {
			newUnitID = id;
			break;
		}
	}

	for (const PlayerStatus& playerStatus : statusOfPlayers) {
		if (playerStatus.type == 'B' && playerStatus.xValue == xValue && playerStatus.yValue == yValue) {

			char production = playerStatus.production;
			for (const auto& stat : unitStats) {
				if (std::get<0>(stat) == production) {
					newUnitEndurance = std::get<1>(stat);
					break;
				}
			}
			break;
		}
	}

	// Create the new unit
	PlayerStatus newUnit = { newUnitAffiliation, unitType, newUnitID, xValue, yValue, newUnitEndurance, '0' };
	statusOfPlayers.push_back(newUnit);

	// Save the updated status to the statusFile
	std::ofstream statusFileTxt(statusFile);
	if (!statusFileTxt.is_open()) {
		std::cout << "Failed to open " << statusFile << " for writing.\n";
		return;
	}

	// Write the updated status to the file
	for (const PlayerStatus& playerStatus : statusOfPlayers) {
		statusFileTxt << playerStatus.affiliation << ' ' << playerStatus.type << ' ' << playerStatus.id << ' '
			<< playerStatus.xValue << ' ' << playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
			<< playerStatus.production << '\n';
	}
	statusFileTxt.close();
}

void buildUnits(std::vector<int>& productionTimer1, std::vector<int>& productionTimer2, int turn, const std::vector<std::tuple<char, int, int, int, int, int>>& unitStats, const std::string& statusFile, long& gold) {

	// Read the status file
	std::ifstream statusTxt(statusFile);
	if (!statusTxt.is_open()) {
		std::cout << "Failed to open " << statusFile << " for reading.\n";
		return;
	}

	// Ignore the first line
	std::string dummyLine;
	std::getline(statusTxt, dummyLine);

	// Read the status data
	std::vector<PlayerStatus> statusOfPlayers;
	std::string line;
	while (std::getline(statusTxt, line)) {
		std::istringstream iss(line);
		PlayerStatus playerStatus;
		iss >> playerStatus.affiliation >> playerStatus.type >> playerStatus.id >> playerStatus.xValue
			>> playerStatus.yValue >> playerStatus.enduranceLeft >> playerStatus.production;

		if (playerStatus.type == 'B') {
			statusOfPlayers.push_back(playerStatus);
		}
	}

	statusTxt.close();

	// Check if the production timer allows building units
	if (productionTimer1[0] == 0 || productionTimer2[0] == 0) {
		// Find the base's production unit type
		char productionUnitType = ' ';
		int baseID = -1;
		int baseXValue = -1;
		int baseYValue = -1;
		int baseXValueE = -1;
		int baseYValueE = -1;
		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.type == 'B' && playerStatus.production != '0' && playerStatus.id == 0) {
				productionUnitType = playerStatus.production;
				baseXValue = playerStatus.xValue;
				baseYValue = playerStatus.yValue;
				baseID = playerStatus.id;
				break;
			}
		}
		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.type == 'B' && playerStatus.production != '0' && playerStatus.id == 1) {
				productionUnitType = playerStatus.production;
				baseXValueE = playerStatus.xValue;
				baseYValueE = playerStatus.yValue;
				baseID = playerStatus.id;
				break;
			}
		}
		
		// Find the corresponding unit stats
		const std::tuple<char, int, int, int, int, int>* unitStat = nullptr;
		for (const auto& stat : unitStats) {
			if (std::get<0>(stat) == productionUnitType) {
				unitStat = &stat;
				break;
			}
		}

		if (unitStat) {
			// Get the number of turns required to build the unit
			int turnsToBuild = std::get<5>(*unitStat);

			if (productionTimer1[0] == 0) {
				// Update production timer 1
				productionTimer1[0] = turnsToBuild + turn;
				productionTimer1[1] = baseXValue;
				productionTimer1[2] = baseYValue;
			}
			else if (productionTimer2[0] == 0) {
				// Update production timer 2
				productionTimer2[0] = turnsToBuild + turn;
				productionTimer2[1] = baseXValueE;
				productionTimer2[2] = baseYValueE;
			}
		}
	}
	// Check if enough turns have passed to build the unit
	std::cout << "Production timer Player: " << (productionTimer1[0] - turn) <<"\n";
	if ((productionTimer1[0] - turn) <= 0) {
		productionTimer1[0] = 0;

		// Find the base's production status
		char productionStatus = '0';
		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.type == 'B' && playerStatus.xValue == productionTimer1[1] && playerStatus.yValue == productionTimer1[2]) {
				productionStatus = playerStatus.production;
				break;
			}
		}

		if (productionStatus != '0') {
			// Create a new active unit based on the production status
			createNewUnit(productionStatus, productionTimer1[1], productionTimer1[2], unitStats, statusFile, turn);

			// Read the status file
			std::ifstream statusTxt(statusFile);
			if (!statusTxt.is_open()) {
				std::cout << "Failed to open " << statusFile << " for reading.\n";
				return;
			}

			// Read the status data
			std::vector<PlayerStatus> statusOfPlayersFull;
			std::string line;
			while (std::getline(statusTxt, line)) {
				std::istringstream iss(line);
				PlayerStatus playerStatus;
				iss >> playerStatus.affiliation >> playerStatus.type >> playerStatus.id >> playerStatus.xValue
					>> playerStatus.yValue >> playerStatus.enduranceLeft >> playerStatus.production;
				statusOfPlayersFull.push_back(playerStatus);
			}

			statusTxt.close();

			// Change the production status of the base to '0'
			for (PlayerStatus& playerStatus : statusOfPlayersFull) {
				if (playerStatus.type == 'B' && playerStatus.xValue == productionTimer1[1] && playerStatus.yValue == productionTimer1[2]) {
					playerStatus.production = '0';
					
					break;
				}
			}

			// Save the updated status to the statusFile
			std::ofstream statusFileTxt(statusFile);
			if (!statusFileTxt.is_open()) {
				std::cout << "Failed to open " << statusFile << " for writing.\n";
				return;
			}

			// Update the gold value in the first line
			statusFileTxt << gold << '\n';

			// Write the updated status to the file
			for (const PlayerStatus& playerStatus : statusOfPlayersFull) {
				statusFileTxt << playerStatus.affiliation << ' ' << playerStatus.type << ' ' << playerStatus.id << ' '
					<< playerStatus.xValue << ' ' << playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
					<< playerStatus.production << '\n';
			}

			statusFileTxt.close();

		}
	}
	// Check if enough turns have passed to build the unit
	std::cout << "Production timer Enemy: " << (productionTimer2[0] - turn) << "\n";
	if ((productionTimer2[0] - turn) <= 0) {
		productionTimer2[0] = 0;
		
		// Find the base's production status
		char productionStatus = '0';
		for (const PlayerStatus& playerStatus : statusOfPlayers) {
			if (playerStatus.type == 'B' && playerStatus.xValue == productionTimer2[1] && playerStatus.yValue == productionTimer2[2]) {
				productionStatus = playerStatus.production;
				break;
			}
		}

		if (productionStatus != '0') {
			// Create a new active unit based on the production status
			createNewUnit(productionStatus, productionTimer2[1], productionTimer2[2], unitStats, statusFile, turn);
			// Read the status file
			std::ifstream statusTxt(statusFile);
			if (!statusTxt.is_open()) {
				std::cout << "Failed to open " << statusFile << " for reading.\n";
				return;
			}

			// Read the status data
			std::vector<PlayerStatus> statusOfPlayersFull;
			std::string line;
			while (std::getline(statusTxt, line)) {
				std::istringstream iss(line);
				PlayerStatus playerStatus;
				iss >> playerStatus.affiliation >> playerStatus.type >> playerStatus.id >> playerStatus.xValue
					>> playerStatus.yValue >> playerStatus.enduranceLeft >> playerStatus.production;
				statusOfPlayersFull.push_back(playerStatus);
			}

			statusTxt.close();

			// Change the production status of the base to '0'
			for (PlayerStatus& playerStatus : statusOfPlayersFull) {
				if (playerStatus.type == 'B' && playerStatus.xValue == productionTimer2[1] && playerStatus.yValue == productionTimer2[2]) {
					playerStatus.production = '0';

					break;
				}
			}

			// Save the updated status to the statusFile
			std::ofstream statusFileTxt(statusFile);
			if (!statusFileTxt.is_open()) {
				std::cout << "Failed to open " << statusFile << " for writing.\n";
				return;
			}

			// Update the gold value in the first line
			statusFileTxt << gold << '\n';

			// Write the updated status to the file
			for (const PlayerStatus& playerStatus : statusOfPlayersFull) {
				statusFileTxt << playerStatus.affiliation << ' ' << playerStatus.type << ' ' << playerStatus.id << ' '
					<< playerStatus.xValue << ' ' << playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
					<< playerStatus.production << '\n';
			}

			statusFileTxt.close();

		}
	}
	return;
}

void swapAffiliations(const std::string& statusFile, long gold) {
	// Read the status file
	std::ifstream statusTxt(statusFile);
	if (!statusTxt.is_open()) {
		std::cout << "Failed to open " << statusFile << " for reading.\n";
		return;
	}

	// Skip the first line
	std::string dummyLine;
	std::getline(statusTxt, dummyLine);

	// Read the status data and update affiliations
	std::vector<std::string> updatedStatusData;
	std::string line;
	while (std::getline(statusTxt, line)) {
		if (line.empty()) {
			continue; // Skip empty lines, if any
		}

		// Swap affiliations in the first column
		if (line[0] == 'P') {
			line[0] = 'E';
		}
		else if (line[0] == 'E') {
			line[0] = 'P';
		}

		updatedStatusData.push_back(line);
	}

	statusTxt.close();

	// Open the status file for writing
	std::ofstream updatedStatusFile(statusFile);
	if (!updatedStatusFile.is_open()) {
		std::cout << "Failed to open " << statusFile << " for writing.\n";
		return;
	}

	// Write the gold value to the first line
	updatedStatusFile << gold << '\n';

	// Write the updated status data to the file
	for (const std::string& updatedLine : updatedStatusData) {
		updatedStatusFile << updatedLine << '\n';
	}

	updatedStatusFile.close();
}

Mediator::Mediator() {
	
};

void Mediator::startGame(const std::string& programName, const std::string& programNameE, int maxGameTurns,
	double maxTimeLimit, double maxTimeLimitE, std::vector<std::vector<char>> map, const std::string& statusFile,
	const std::string& ordersFile, const std::string& mapFile) {
	long gold = 2000;
	long goldE = 2000;
	std::vector<int> productionTimer1(3, 0); // Three integers initialized to 0
	std::vector<int> productionTimer2(3, 0); // Three integers initialized to 0
	std::vector<std::string> currentOrders;
	std::vector<std::string> currentOrdersE;
	std::vector<std::tuple<std::string, int, std::string>> allOrders;

	// Open the status file and clear its content
	std::ofstream statusTxt(statusFile);
	if (!statusTxt.is_open()) {
		// Handle file opening error
		return;
	}

	// Find the positions of '1' and '2' on the map
	int xValue1 = -1;
	int yValue1 = -1;
	int xValue2 = -1;
	int yValue2 = -1;
	for (int i = 0; i < map.size(); ++i) {
		for (int j = 0; j < map[i].size(); ++j) {
			if (map[i][j] == '1') {
				xValue1 = j/2;
				yValue1 = i;
			}
			else if (map[i][j] == '2') {
				xValue2 = j/2;
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

	// Write gold value to the first line
	statusTxt << gold << '\n';

	// Write the initial player base statuses to the status file
	std::vector<PlayerStatus> statusOfPlayers = createStatusOfPlayers(xValue1, yValue1, xValue2, yValue2);
	
	for (const PlayerStatus& playerStatus : statusOfPlayers) {
		statusTxt << playerStatus.affiliation << ' ' << playerStatus.type << ' '
			<< playerStatus.id << ' ' << playerStatus.xValue << ' '
			<< playerStatus.yValue << ' ' << playerStatus.enduranceLeft << ' '
			<< playerStatus.production << '\n';
	}

	statusTxt.close();
	
	for (int turn = 1; turn <= maxGameTurns; turn++) {
		std::cout << "Turn " << turn << "\n";

		// Run Player on odd iterations
		if (turn % 2 == 1) {

			// Start timer
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

			std::string command = programName + " " + mapFile + " " + statusFile + " " + ordersFile;
			std::system(command.c_str());

			// Stop timer and calculate elapsed time
			std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedSeconds = endTime - startTime;
			std::cout << "Elapsed seconds for Player: " << elapsedSeconds.count() << "\n";

			// Check if elapsed time exceeds the time limit
			if (elapsedSeconds.count() > (maxTimeLimit != 0 ? maxTimeLimit : 5)) {
				std::cout << "Time limit player 1\n";
				break; // Break the loop if time limit is exceeded
			}

			// Read player orders
			readOrders(ordersFile, currentOrders, allOrders, turn);

			// Search for critical errors
			//searchForErrors(statusOfPlayers, currentOrders, currentOrdersE, gold, goldE, map, allOrders);
			
			// Update current state
			updateState(unitStats, damageType, damageValue, statusFile, currentOrders, map, turn, gold, allOrders);

			// Build units in bases after a number of turns
			//buildUnits(productionTimer1, productionTimer2, turn, unitStats, statusFile, gold);

			// Read and print the contents of the updated status file
			std::ifstream file(statusFile);
			if (!file.is_open()) {
				std::cout << "Failed to open " << statusFile << " for reading.\n";
				return;
			}

			std::string line;
			while (std::getline(file, line)) {
				std::cout << line << '\n';
			}

			file.close();

			// Clear the vector of current orders
			currentOrders.clear();

			// Swaps affiliations
			swapAffiliations(statusFile, gold);
		}

		// Run Enemy on even iterations
		if (turn % 2 == 0) {

			// Start timer
			std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

			std::string command = programNameE + " " + mapFile + " " + statusFile + " " + ordersFile;
			std::system(command.c_str());

			// Stop timer and calculate elapsed time
			std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsedSeconds = endTime - startTime;
			std::cout << "Elapsed seconds for Enemy: " << elapsedSeconds.count() << "\n";

			// Check if elapsed time exceeds the time limit
			if (elapsedSeconds.count() > (maxTimeLimitE != 0 ? maxTimeLimitE : 5)) {
				std::cout << "Time limit player 2\n";
				break; // Break the loop if time limit is exceeded
			}

			// Read enemy orders
			readOrders(ordersFile, currentOrdersE, allOrders, turn);

			// Search for critical errors
			//searchForErrors(statusOfPlayers, currentOrders, currentOrdersE, gold, goldE, map, allOrders);

			// Update current state
			updateState(unitStats, damageType, damageValue, statusFile, currentOrdersE, map, turn, goldE, allOrders);

			// Build units in bases after a number of turns
			//buildUnits(productionTimer1, productionTimer2, turn, unitStats, statusFile, goldE);

			// Read and print the contents of the updated status file
			std::ifstream file(statusFile);
			if (!file.is_open()) {
				std::cout << "Failed to open " << statusFile << " for reading.\n";
				return;
			}

			std::string line;
			while (std::getline(file, line)) {
				std::cout << line << '\n';
			}

			file.close();

			// Clear the vector of current orders
			currentOrdersE.clear();

			// Swaps affiliations
			swapAffiliations(statusFile, goldE);
		}
	}
};

