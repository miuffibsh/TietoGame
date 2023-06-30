#ifndef MAP_H
#define MAP_H

#include <vector>
#include <utility>

class Map
{
public:
	Map(int x, int y);
	~Map();
	void generateRandomMap();
	void printMap(const std::string& fileName) const;
	bool isAdjacentToValue(int row, int column, char value);
	std::vector<std::pair<int, int>> findCoordinatesOfBase(char baseNumber);
	bool hasPath(int startRow, int startCol, int endRow, int endCol, char baseNumber);
	bool canReachMinesAndEnemyBase(int rows, int columns, char** array, int rowCoordinate, int colCoordinate, char myBase, char enemyBase);

private:
	int rows;
	int columns;
	char** array;

	bool isValidPosition(int row, int col) {
		return row >= 0 && row < rows && col >= 0 && col < columns;
	}

	bool isObstacle(int row, int col) {
		return array[row][col] == '9';
	}

	bool isBase(int row, int col, char baseNumber) {
		return array[row][col] == baseNumber;
	}

	// List of valid directions: up, down, left, right
	const std::vector<std::pair<int, int>> directions{ {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
};

#endif