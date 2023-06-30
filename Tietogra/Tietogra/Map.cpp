#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <vector>
#include "Map.h"

Map::Map(int x, int y) {
	rows = x;
	columns = y;
	array = new char*[rows];
	for (int i = 0; i < rows; ++i) {
		array[i] = new char[columns];
	}
	// Set the seed for random number generation
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

Map::~Map() {
	for (int i = 0; i < rows; ++i) {
		delete[] array[i];
	}
	delete[] array;
}

void Map::generateRandomMap() {
	// Fill the map with zeroes
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			array[i][j] = '0';
		}
	}

	// 15% (rounded down) of the map consists of obstacles
	int numZeroesToChange = rows * columns * 0.2;

	while (numZeroesToChange > 0) {
		int randomRow = std::rand() % rows;
		int randomCol = std::rand() % columns;

		if (array[randomRow][randomCol] == '0') {
			array[randomRow][randomCol] = '9';
			numZeroesToChange--;
		}
	}

	// 5% (rounded up) of the map consists of mines
	numZeroesToChange = (int(rows * columns * 0.05) > 0) ? int(rows * columns * 0.05) : 1;

	while (numZeroesToChange > 0) {
		int randomRow = std::rand() % rows;
		int randomCol = std::rand() % columns;

		if (array[randomRow][randomCol] == '0') {
			array[randomRow][randomCol] = '6';
			numZeroesToChange--;
		}
	}

	// Place player base '1'
	numZeroesToChange = 1;

	while (numZeroesToChange > 0) {
		int randomRow = std::rand() % rows;
		int randomCol = std::rand() % columns;

		if (array[randomRow][randomCol] == '0') {
			array[randomRow][randomCol] = '1';
			numZeroesToChange--;
		}
	}

	// Place enemy base '2'
	numZeroesToChange = 1;

	// Base '2' can't be adjacent horizontally and vertically to base '1'
	while (numZeroesToChange > 0) {
		int randomRow = std::rand() % rows;
		int randomCol = std::rand() % columns;

		if (array[randomRow][randomCol] == '0' && !isAdjacentToValue(randomRow, randomCol, '1')) {
			array[randomRow][randomCol] = '2';
			numZeroesToChange--;
		}
	}

	// Check if units from base 1 can reach base 2 and mines
	bool baseOneCorrect = false;
	std::vector<std::pair<int, int>> coordinatesOfBase = findCoordinatesOfBase('1');
	for (const auto& coordinate : coordinatesOfBase) {
		int rowCoordinate = coordinate.first;
		int colCoordinate = coordinate.second;
		baseOneCorrect = canReachMinesAndEnemyBase(rows, columns, array, rowCoordinate, colCoordinate, '1', '2');
	}

	// Check if units from base 2 can reach base 1 and mines
	bool baseTwoCorrect = false;
	coordinatesOfBase = findCoordinatesOfBase('2');
	for (const auto& coordinate : coordinatesOfBase) {
		int rowCoordinate = coordinate.first;
		int colCoordinate = coordinate.second;
		baseTwoCorrect = canReachMinesAndEnemyBase(rows, columns, array, rowCoordinate, colCoordinate, '2', '1');
	}

	if (baseOneCorrect == false || baseTwoCorrect == false) {
		generateRandomMap();	// If units can't reach enemy base or mines then generate a new map
	}
}

// Prints the map to the console and to the text file
void Map::printMap(const std::string& fileName) const {
	// Print the map to the console
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			std::cout << array[i][j] << " ";
		}
		std::cout << std::endl;
	}

	// Open the original file for reading
	// It doesn't check if the file exists, because it is checked anyway while generating map
	std::ifstream inputFile(fileName);

	// Open a new file for writing
	std::ofstream outputFile("temp.txt");

	// Copy the necessary lines from the original file to the new file
	std::string line;
	int lineCount = 0;
	while (std::getline(inputFile, line) && lineCount < 2) {
		outputFile << line << std::endl;
		lineCount++;
	}

	outputFile << std::endl;  // Add an empty line
	outputFile << "Wygenerowana mapa:" <<std::endl;

	// Save the map to the new file
	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			outputFile << array[i][j] << " ";
		}
		outputFile << std::endl;
	}

	// Close the files
	inputFile.close();
	outputFile.close();

	// Remove the original file
	std::remove(fileName.c_str());

	// Rename the temporary file to the original file name
	std::rename("temp.txt", fileName.c_str());
}

// Check if any adjacent cells contain the specified value
bool Map::isAdjacentToValue(int row, int column, char value) {

	// Check left
	if (column > 0 && array[row][column - 1] == value) {
		return true;
	}

	// Check right
	if (column < columns - 1 && array[row][column + 1] == value) {
		return true;
	}

	// Check up
	if (row > 0 && array[row - 1][column] == value) {
		return true;
	}

	// Check down
	if (row < rows - 1 && array[row + 1][column] == value) {
		return true;
	}

	return false;
}

// Find the coordinates of base in the array
std::vector<std::pair<int, int>> Map::findCoordinatesOfBase(char baseNumber) {
	std::vector<std::pair<int, int>> coordinates;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < columns; ++j) {
			if (array[i][j] == baseNumber) {
				coordinates.push_back({ i, j });
			}
		}
	}

	return coordinates;
}

// Perform breadth-first search to determine if there is a path between two positions on the map
bool Map::hasPath(int startRow, int startCol, int endRow, int endCol, char baseNumber) {
	std::vector<std::vector<bool>> visited(rows, std::vector<bool>(columns, false));
	std::queue<std::pair<int, int>> q;

	// Check if the starting position is valid and not an obstacle or base
	if (!isValidPosition(startRow, startCol) || isObstacle(startRow, startCol) || isBase(startRow, startCol, baseNumber)) {
		return false;
	}

	visited[startRow][startCol] = true;
	q.push({ startRow, startCol });

	while (!q.empty()) {
		auto current = q.front();
		q.pop();

		int row = current.first;
		int col = current.second;

		// Check if the end position is reached
		if (row == endRow && col == endCol) {
			return true;
		}

		// Explore adjacent positions (horizontally and vertically)
		for (auto dir : directions) {
			int newRow = row + dir.first;
			int newCol = col + dir.second;

			// Check if the new position is valid and not visited, an obstacle, or a base
			if (isValidPosition(newRow, newCol) && !visited[newRow][newCol] && !isObstacle(newRow, newCol) && !isBase(newRow, newCol, baseNumber)) {
				visited[newRow][newCol] = true;
				q.push({ newRow, newCol });
			}
		}
	}

	// No path found
	return false;
}

// Check accessibility from base one to mines and base two
bool Map::canReachMinesAndEnemyBase(int rows, int columns, char** array, int rowCoordinate, int colCoordinate, char myBase, char enemyBase) {
	bool canReachMinesAndBaseTwo = true;
	for (int row = 0; row < rows; ++row) {
		for (int col = 0; col < columns; ++col) {
			if (array[row][col] == myBase || array[row][col] == '6') {
				if (!hasPath(row, col, rowCoordinate, colCoordinate, enemyBase)) {
					return false;
				}
			}
		}
	}
	return true;
}