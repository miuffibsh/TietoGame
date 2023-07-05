#include "OrdersAndAI.h"
#include <fstream>
#include <iostream>
#include <ctime>
#include <queue>
#include <climits>
#include <limits>
#include <random>

struct UnitBaseInfo {
	int unitID;
	int colValue;
	int rowValue;
};

struct Point {
	int row;
	int col;
};

bool isValidMove(int row, int col, const std::vector<std::vector<char>>& mapArray) {
	int numRows = mapArray.size();
	int numCols = mapArray[0].size();
	return (row >= 0 && row < numRows && col >= 0 && col < numCols && mapArray[row][col] != '9');
}

bool isObstacle(int col, int row, const std::vector<Base>& bases, const std::vector<ActiveUnit>& activeUnits, const std::vector<std::vector<char>>& mapArray) {
	// Check if the position is an enemy base
	for (const auto& base : bases) {
		if (base.unitAffiliation == 'E' && base.colValue == col && base.rowValue == row) {
			return true;
		}
	}

	// Check if the position is an enemy unit
	for (const auto& unit : activeUnits) {
		if (unit.unitAffiliation == 'E' && unit.colValue == col && unit.rowValue == row) {
			return true;
		}
	}

	// Check if the position is an obstacle '9'
	char cell = mapArray[row][col];
	if (cell == '9') {
		return true;
	}
	return false;
}

std::vector<char> findShortestPath(const std::vector<std::vector<char>>& mapArray, int targetColumn, int targetRow,
	const std::vector<Base>& bases, const std::vector<ActiveUnit>& activeUnits, int initialCol, int initialRow) {

	int numRows = mapArray.size();
	int numCols = mapArray[0].size();

	// Check if initialCol and initialRow are within valid range
	if (initialRow < 0 || initialRow >= numRows || initialCol < 0 || initialCol >= numCols) {
		// Handle invalid initial position
		// Return an empty path or throw an exception
		return {};
	}

	std::vector<std::vector<bool>> visited(numRows, std::vector<bool>(numCols, false));
	std::vector<std::vector<Point>> parent(numRows, std::vector<Point>(numCols));
	std::queue<Point> bfsQueue;
	
	bfsQueue.push({ initialRow, initialCol });
	visited[initialRow][initialCol] = true;

	int dr[4] = { -1, 1, 0, 0 };
	int dc[4] = { 0, 0, -1, 1 };
	char directions[4] = { 'D', 'U', 'R', 'L' };

	while (!bfsQueue.empty()) {
		Point current = bfsQueue.front();
		bfsQueue.pop();

		int row = current.row;
		int col = current.col;

		if (row == targetRow && col == targetColumn) {
			// Reconstruct the path
			std::vector<char> path;
			while (row != initialRow || col != initialCol) {
				Point prev = parent[row][col];
				int drIndex = prev.row - row;
				int dcIndex = prev.col - col;
				for (int i = 0; i < 4; ++i) {
					if (dr[i] == drIndex && dc[i] == dcIndex) {
						path.push_back(directions[i]);
						break;
					}
				}
				row = prev.row;
				col = prev.col;
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		// Check all four adjacent cells (up, down, left, right)
		for (int i = 0; i < 4; ++i) {
			int newRow = row + dr[i];
			int newCol = col + dc[i];

			if (isValidMove(newRow, newCol, mapArray) && !visited[newRow][newCol]) {
				bfsQueue.push({ newRow, newCol });
				visited[newRow][newCol] = true;
				parent[newRow][newCol] = { row, col };
			}
		}
	}

	// If no path is found, return an empty path
	return {};
}

int getUnitSpeed(char unitType, const std::vector<UnitStats>& unitStats) {
	// Find the speed of the unit based on its unit type
	for (const auto& stats : unitStats) {
		if (stats.type == unitType) {
			return stats.speed;
		}
	}
	// Return a default value if the unit type is not found
	return 0;
}

int getUnitRange(char unitType, const std::vector<UnitStats>& unitStats) {
	// Find the speed of the unit based on its unit type
	for (const auto& stats : unitStats) {
		if (stats.type == unitType) {
			return stats.range;
		}
	}
	// Return a default value if the unit type is not found
	return 0;
}

int findEnemyUnitID(const std::vector<ActiveUnit>& activeUnits, int targetX, int targetY) {
	for (const auto& enemyUnit : activeUnits) {
		if (enemyUnit.unitAffiliation == 'E' && enemyUnit.colValue == targetX && enemyUnit.rowValue == targetY) {
			return enemyUnit.ID;
		}
	}
	return -1;  // Return -1 if no enemy unit is found at the specified position
}

OrdersAndAI::OrdersAndAI() {
	int executedCommands = 0; // Initialize member of commands
}

void OrdersAndAI::orderBase(std::vector<ActiveUnit>& activeUnits, long& gold, int baseID, const std::vector<UnitStats>& unitStats) {
	// Define the chance matrix for each unit type based on gold value
	std::vector<std::vector<int>> chanceOfType = {
		{3, 6, 4, 3, 1, 2, 1},    // {K, S, A, P, R, C, W}, when gold >= 2000
		{3, 5, 4, 2, 1, 1, 4},    // gold 1000-1999
		{3, 6, 4, 2, 0, 0, 5},    // gold 500-999
		{0, 6, 5, 3, 0, 0, 6},    // gold 400-499
		{0, 5, 4, 1, 0, 0, 10},   // gold 101-399
		{0, 0, 0, 0, 0, 0, 1}     // gold 100
	};

	// Get the index of the chance matrix based on the gold value
	int chanceIndex = 0;
	if (gold >= 2000)
		chanceIndex = 0;
	else if (gold >= 1000 && gold < 2000)
		chanceIndex = 1;
	else if (gold >= 500 && gold < 1000)
		chanceIndex = 2;
	else if (gold >= 400 && gold < 500)
		chanceIndex = 3;
	else if (gold >= 101 && gold < 400)
		chanceIndex = 4;
	else if (gold <= 100)
		chanceIndex = 5;

	// Create a 1-dimensional char array based on the chance matrix
	std::vector<char> unitTypes;
	const std::vector<UnitStats>& actorUnitStats = unitStats;

	for (int i = 0; i < chanceOfType[chanceIndex].size(); ++i) {
		int count = chanceOfType[chanceIndex][i];
		char unitType = actorUnitStats[i].type;
		unitTypes.insert(unitTypes.end(), count, unitType);
	}

	// Randomly choose a unit type from the 1-dimensional char array
	// Set the seed for random number generation
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	char chosenUnitType = unitTypes[rand() % unitTypes.size()];

	// Find the cost of the chosen unit type
	int costOfChosenUnit = 0;
	for (const auto& unit : unitStats) {
		if (unit.type == chosenUnitType) {
			costOfChosenUnit = unit.cost;
			break;
		}
	}

	// Ensure gold minus the cost of the chosen unit type is never lower than 100, unless it's type 'W'
	if (chosenUnitType != 'W' && (gold - costOfChosenUnit) < 100) {
		orderBase(activeUnits, gold, baseID, unitStats);
		return; // Return to avoid adding the production command multiple times
	}

	// Add the command for producing the chosen unit to the executedCommands vector
	Command productionCommand;
	productionCommand.action = ActionType::Produce;
	productionCommand.unitType = chosenUnitType;
	productionCommand.baseID = baseID;

	executedCommands.push_back(productionCommand);
}

void OrdersAndAI::actionUnits(std::vector<ActiveUnit>& activeUnits, std::vector<std::vector<char>>& mapArray,
	const std::vector<UnitStats>& unitStats, const std::vector<Base>& bases, const std::vector<char>& damageType, const std::vector<std::vector<int>>& damageValue) {
	// Performs movement and attacks of units
	Actors actors;
	Command moveCommand;
	moveCommand.action = ActionType::Move;
	Command attackCommand;
	attackCommand.action = ActionType::Attack;

	// Create a vector to store the extracted values
	std::vector<UnitBaseInfo> enemyBaseUnits;

	// Iterate over the activeUnits vector to get information about enemy base
	for (const auto& unit : bases) {
		// Check if the unit is of type 'B' and has an affiliation of 'E'
		if (unit.unitAffiliation == 'E') {
			// Create a new UnitInfo object and populate it with the unit information
			UnitBaseInfo info;
			info.unitID = unit.ID;
			info.colValue = unit.colValue;
			info.rowValue = unit.rowValue;

			// Add the UnitInfo object to the vector
			enemyBaseUnits.push_back(info);

			break;
		}
	}

	// Check every player unit and choose their actions
	for (auto& unit : activeUnits) {
		// Check if the unit has player affiliation ('P'); for Worker
		if (unit.unitAffiliation == 'P') {
			// Check if the unit is of type 'W'
			if (unit.unitType == 'W') {
				int startX = unit.colValue;
				int startY = unit.rowValue;
				
				int targetX = -1;
				int targetY = -1;
				int minDistance = std::numeric_limits<int>::max();

				// Find the position of the target '6' on the map
				for (size_t i = 0; i < mapArray.size(); ++i) {
					for (size_t j = 0; j < mapArray[i].size(); ++j) {
						if (mapArray[i][j] == '6') {
							int distance = std::abs(static_cast<int>(j) - startX) + std::abs(static_cast<int>(i) - startY);
							if (distance < minDistance) {
								targetX = j;
								targetY = i;
								minDistance = distance;
							}
						}
					}
				}
				
				// Perform pathfinding to find the optimal route to the target
				std::vector<char> optimalPath = findShortestPath(mapArray, targetX, targetY, bases, activeUnits, unit.colValue, unit.rowValue);
				
				// Limit the number of movements based on unit speed
				int speed = getUnitSpeed(unit.unitType, unitStats);
				int movements = 0;

				// Move the unit along the optimal path
				for (char direction : optimalPath) {
					if (movements < speed) {
						actors.move(mapArray, activeUnits, bases, direction, unit.ID);
						--speed;
					}
				}

				// Check for adjacent enemy units to the mine
				int enemyID = -1;

				if (mapArray[unit.colValue][unit.rowValue] == '6') {
					// Check adjacent positions for enemy units
					std::vector<int> adjacentEnemyIDs;

					if (isValidMove(unit.colValue - 1, unit.rowValue, mapArray) && mapArray[unit.colValue - 1][unit.rowValue] != ' ' && mapArray[unit.colValue - 1][unit.rowValue] != 'P') {
						int enemy = findEnemyUnitID(activeUnits, unit.rowValue, unit.colValue - 1);
						if (enemy != -1) {
							adjacentEnemyIDs.push_back(enemy);
						}
					}
					if (isValidMove(unit.colValue + 1, unit.rowValue, mapArray) && mapArray[unit.colValue + 1][unit.rowValue] != ' ' && mapArray[unit.colValue + 1][unit.rowValue] != 'P') {
						int enemy = findEnemyUnitID(activeUnits, unit.rowValue, unit.colValue + 1);
						if (enemy != -1) {
							adjacentEnemyIDs.push_back(enemy);
						}
					}
					if (isValidMove(unit.colValue, unit.rowValue - 1, mapArray) && mapArray[unit.colValue][unit.rowValue - 1] != ' ' && mapArray[unit.colValue][unit.rowValue - 1] != 'P') {
						int enemy = findEnemyUnitID(activeUnits, unit.rowValue - 1, unit.colValue);
						if (enemy != -1) {
							adjacentEnemyIDs.push_back(enemy);
						}
					}
					if (isValidMove(unit.colValue, unit.rowValue + 1, mapArray) && mapArray[unit.colValue][unit.rowValue + 1] != ' ' && mapArray[unit.colValue][unit.rowValue + 1] != 'P') {
						int enemy = findEnemyUnitID(activeUnits, unit.rowValue + 1, unit.colValue);
						if (enemy != -1) {
							adjacentEnemyIDs.push_back(enemy);
						}
					}

					if (!adjacentEnemyIDs.empty()) {
						// Choose a random enemy from the adjacent enemies
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> distr(0, adjacentEnemyIDs.size() - 1);
						enemyID = adjacentEnemyIDs[distr(gen)];
					}
				}
				else {
					// Check if player unit is adjacent to '6' on the map
					if (isValidMove(unit.colValue - 1, unit.rowValue, mapArray) && mapArray[unit.colValue - 1][unit.rowValue] == '6') {
						enemyID = findEnemyUnitID(activeUnits, unit.rowValue, unit.colValue - 1);
					}
					else if (isValidMove(unit.colValue + 1, unit.rowValue, mapArray) && mapArray[unit.colValue + 1][unit.rowValue] == '6') {
						enemyID = findEnemyUnitID(activeUnits, unit.rowValue, unit.colValue + 1);
					}
					else if (isValidMove(unit.colValue, unit.rowValue - 1, mapArray) && mapArray[unit.colValue][unit.rowValue - 1] == '6') {
						enemyID = findEnemyUnitID(activeUnits, unit.rowValue - 1, unit.colValue);
					}
					else if (isValidMove(unit.colValue, unit.rowValue + 1, mapArray) && mapArray[unit.colValue][unit.rowValue + 1] == '6') {
						enemyID = findEnemyUnitID(activeUnits, unit.rowValue + 1, unit.colValue);
					}
				}

				// Attack the enemy if found and unit speed is greater than 0
				if (enemyID != -1 && speed > 0) {
					actors.attack(activeUnits, unit.ID, enemyID);
					speed = 0;
				}

				moveCommand.unitID = unit.ID;
				moveCommand.finalCol = unit.colValue;
				moveCommand.finalRow = unit.rowValue;
				executedCommands.push_back(moveCommand);  // Add the executed command for Move

				if (enemyID != -1) {
					attackCommand.unitID = unit.ID;
					attackCommand.enemyID = enemyID;
					executedCommands.push_back(attackCommand);  // Add the executed command for Attack
				}
			}

		}
		
		// Check if the unit has player affiliation ('P'); for other types
		if (unit.unitAffiliation == 'P') {
		// Check if the unit is not of type 'W' and not of type 'B'
			if (unit.unitType != 'W' && unit.unitType != 'B') {
				int startX = unit.colValue;
				int startY = unit.rowValue;

				int targetX = -1;
				int targetY = -1;

				int minDistance = std::numeric_limits<int>::max();
				int distance = 0;

				// Find the nearest enemy unit
				for (const auto& enemyUnit : activeUnits) {
					// Check if the unit has enemy affiliation ('E')
					if (enemyUnit.unitAffiliation == 'E') {
						// Check if the enemy unit is of type 'W'
						if (enemyUnit.unitType == 'W') {
							continue;  // Skip 'W' units
						}

						int enemyX = enemyUnit.colValue;
						int enemyY = enemyUnit.rowValue;
						
						// Calculate the distance between the units
						distance = std::abs(startX - enemyX) + std::abs(startY - enemyY);

						// Check if the distance is smaller than the current minimum distance
						if (distance < minDistance) {
							minDistance = distance;
							targetX = enemyX;
							targetY = enemyY;
						}
					}
				}
				
				// Find the adjacent position nearest to the enemy unit
				int adjacentX = targetX;
				int adjacentY = targetY;
				int shortestDistance = std::numeric_limits<int>::max();
				int mapHeight = mapArray.size();
				int mapWidth = mapArray[0].size();

				if (distance > 1) {
					// Move to the right if not an obstacle and within bounds
					if ((targetX + 1) > startX && targetX + 1 < mapWidth && mapArray[targetY][targetX + 1] != '9') {
						std::vector<char> optimalPath = findShortestPath(mapArray, (targetX + 1), targetY, bases, activeUnits, startX, startY);
						int newDistance = optimalPath.size();
						if (newDistance < shortestDistance && newDistance != 0) {
							++adjacentX;
							shortestDistance = newDistance;
						}
					}
					// Move to the left if not an obstacle and within bounds
					else if ((targetX - 1) < startX && targetX - 1 >= 0 && mapArray[targetY][targetX - 1] != '9') {
						std::vector<char> optimalPath = findShortestPath(mapArray, (targetX - 1), targetY, bases, activeUnits, startX, startY);
						int newDistance = optimalPath.size();
						if (newDistance < shortestDistance && newDistance != 0) {
							--adjacentX;
							shortestDistance = newDistance;
						}
					}
					// Move down if not an obstacle and within bounds
					else if ((targetY + 1) > startY && targetY + 1 < mapHeight && mapArray[targetY + 1][targetX] != '9') {
						std::vector<char> optimalPath = findShortestPath(mapArray, targetX, (targetY + 1), bases, activeUnits, startX, startY);
						int newDistance = optimalPath.size();
						if (newDistance < shortestDistance && newDistance != 0) {
							++adjacentY;
							shortestDistance = newDistance;
						}
					}
					// Move up if not an obstacle and within bounds
					else if ((targetY - 1) < startY && targetY - 1 >= 0 && mapArray[targetY - 1][targetX] != '9') {
						std::vector<char> optimalPath = findShortestPath(mapArray, targetX, (targetY - 1), bases, activeUnits, startX, startY);
						int newDistance = optimalPath.size();
						if (newDistance < shortestDistance && newDistance != 0) {
							--adjacentY;
							shortestDistance = newDistance;
						}
					}
				}
				
				// Perform pathfinding to find the optimal route to the adjacent position
				std::vector<char> optimalPath = findShortestPath(mapArray, adjacentX, adjacentY, bases, activeUnits, unit.colValue, unit.rowValue);

				// Limit the number of movements based on unit speed
				int speed = getUnitSpeed(unit.unitType, unitStats);
				int movements = 0;
				bool performedMove = false;

				// Move the unit along the optimal path
				// Check for enemies in range of player's units
				std::vector<int> enemiesInRange;
				for (char direction : optimalPath) {
					for (const auto& enemyUnit : activeUnits) {
						if (enemyUnit.unitAffiliation == 'E') {
							const int range = getUnitRange(unit.unitType, unitStats);
							const int distance = std::abs(unit.colValue - enemyUnit.colValue) + std::abs(unit.rowValue - enemyUnit.rowValue);
							if (range >= distance) {
								enemiesInRange.push_back(enemyUnit.ID);
							}
						}
					}
					if (movements < speed && enemiesInRange.empty()) {
						actors.move(mapArray, activeUnits, bases, direction, unit.ID);
						performedMove = true;
						--speed;
					}
				}

				// Find the nearest enemy unit among the enemies in range
				int nearestEnemyID = -1;
				int nearestDistance = std::numeric_limits<int>::max();

				for (const auto& enemyID : enemiesInRange) {
					for (const auto& enemyUnit : activeUnits) {
						if (enemyUnit.ID == enemyID) {
							const int distance = std::abs(unit.colValue - enemyUnit.colValue) + std::abs(unit.rowValue - enemyUnit.rowValue);
							if (distance < nearestDistance) {
								nearestEnemyID = enemyID;
								nearestDistance = distance;
							}
							break; // Break the inner loop since we found the matching unit
						}
					}
				}
				
				// Randomly choose an enemyID if there are multiple nearest enemies
				if (!enemiesInRange.empty() && nearestEnemyID == -1) {
					std::vector<int> nearestEnemies;
					for (const auto& enemyID : enemiesInRange) {
						const auto& enemyUnit = activeUnits[enemyID];
						// Add every other unit type except Worker twice, so Worker would have a lower chance of being selected
						if (enemyUnit.unitType == 'W') {
							nearestEnemies.push_back(enemyID);
						}
						else {
							nearestEnemies.push_back(enemyID);
							nearestEnemies.push_back(enemyID);
						}
					}
					if (!nearestEnemies.empty()) {
						std::random_device rd;
						std::mt19937 gen(rd());
						std::uniform_int_distribution<> dis(0, nearestEnemies.size() - 1);
						nearestEnemyID = nearestEnemies[dis(gen)];
					}
					else {
						nearestEnemyID = enemiesInRange[0];
					}
				}

				bool baseAttacked = false;
				int baseAttackedID = -1;
				
				// Check for enemy base within range and attack if possible
				for (const auto& unitInfo : enemyBaseUnits) {
					const int distance = std::abs(unit.colValue - unitInfo.colValue) + std::abs(unit.rowValue - unitInfo.rowValue);

					if (distance <= getUnitRange(unit.unitType, unitStats) && speed > 0) {
						// Perform attack based on the attack chance
						if ((std::rand() % 2) == 0) {
							baseAttacked = true;
							baseAttackedID = unitInfo.unitID;
							speed = 0;
							break;
						}
					}
				}

				bool standardAttack = false;
				// Check for enemy unit within range and attack if possible
				if (nearestEnemyID != -1 && speed > 0) {
					actors.attack(activeUnits, unit.ID, nearestEnemyID);
					standardAttack = true;
					speed = 0;
				}

				if (performedMove) {
					moveCommand.unitID = unit.ID;
					moveCommand.finalCol = unit.colValue;
					moveCommand.finalRow = unit.rowValue;
					executedCommands.push_back(moveCommand);  // Add the executed command for Move
				}

				if (baseAttacked) {
					attackCommand.unitID = unit.ID;
					attackCommand.enemyID = baseAttackedID;
					executedCommands.push_back(attackCommand);  // Add the executed command for Attack
				}

				if (standardAttack) {
					attackCommand.unitID = unit.ID;
					attackCommand.enemyID = nearestEnemyID;
					executedCommands.push_back(attackCommand);  // Add the executed command for Attack
				}
			}
		}
	}
};

void OrdersAndAI::writeOrdersToFile() {
	std::ofstream outputFile("rozkazy.txt");
	if (!outputFile) {
		std::cerr << "Failed to open rozkazy.txt file for writing." << std::endl;
		return;
	}

	// Write commands for each executed action
	for (const auto& command : executedCommands) {
		if (command.action == ActionType::Move) {
			outputFile << command.unitID << " M " << command.finalCol << " " << command.finalRow << std::endl;
		}
		else if (command.action == ActionType::Attack) {
			outputFile << command.unitID << " A " << command.enemyID << std::endl;
		}
		else if (command.action == ActionType::Produce) {
			outputFile << command.baseID << " B " << command.unitType << std::endl;
		}
	}

	outputFile.close();
}

