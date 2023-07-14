#include "OrdersAndAI.h"

void readMapFromFile(const std::string& fileName, std::vector<std::vector<char>>& mapArray, int& rows, int& columns) {
	std::ifstream inputFile(fileName);
	std::string line;

	// Check if "Wygenerowana mapa:" line exists
	bool foundHeader = false;
	while (std::getline(inputFile, line)) {
		if (line.find("Wygenerowana mapa:") != std::string::npos) {
			foundHeader = true;
			break;
		}
	}

	// Read the map numbers and store them in the vector
	rows = 0;
	columns = 0;
	if (foundHeader) {
		while (std::getline(inputFile, line)) {
			std::istringstream iss(line);
			char number;
			std::vector<char> row;
			while (iss >> number) {
				row.push_back(number);
				++columns;
			}
			if (!row.empty()) {
				mapArray.push_back(row);
				++rows;
				columns = 0;
			}
		}
	}
	else {
		// Reset the file stream and read all lines
		inputFile.clear();
		inputFile.seekg(0, std::ios::beg);

		while (std::getline(inputFile, line)) {
			std::istringstream iss(line);
			char number;
			std::vector<char> row;
			while (iss >> number) {
				row.push_back(number);
				++columns;
			}
			if (!row.empty()) {
				mapArray.push_back(row);
				++rows;
				columns = 0;
			}
		}
	}
}

void readStatusFromFile(const std::string& fileName, long& gold, std::vector<Base>& bases, std::vector<ActiveUnit>& activeUnits, char& playerBaseID) {
    std::ifstream inputFile(fileName);
    std::string line;

    // Read the first line as gold
    std::getline(inputFile, line);
    gold = std::stol(line);

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        char unitAffiliation;
        char unitType;
        int ID;
        int colValue;
        int rowValue;
        int enduranceLeft;
        char producing;

        iss >> unitAffiliation >> unitType >> ID >> colValue >> rowValue >> enduranceLeft >> producing;

        // Check if the line represents a base
        if (unitType == 'B') {
            Base base;
            base.unitAffiliation = unitAffiliation;
            base.unitType = unitType;
            base.ID = ID;
            base.colValue = colValue;
            base.rowValue = rowValue;
            base.enduranceLeft = enduranceLeft;
            base.producing = producing;
            bases.push_back(base);

            // Check if the base line starts with 'P' to identify the player base
            if (unitAffiliation == 'P') {
                playerBaseID = ID;
            }

            // Also store the unit type 'B' in activeUnits
            ActiveUnit unit;
            unit.unitAffiliation = unitAffiliation;
            unit.unitType = unitType;
            unit.ID = ID;
            unit.colValue = colValue;
            unit.rowValue = rowValue;
            unit.enduranceLeft = enduranceLeft;
            activeUnits.push_back(unit);
        } else {
            ActiveUnit unit;
            unit.unitAffiliation = unitAffiliation;
            unit.unitType = unitType;
            unit.ID = ID;
            unit.colValue = colValue;
            unit.rowValue = rowValue;
            unit.enduranceLeft = enduranceLeft;
            activeUnits.push_back(unit);
        }
    }
}

int main(int argc, char* argv[]) {
	std::string mapFile, statusFile, ordersFile;

	// Check if command-line arguments for map file, status file, and orders file are provided
	if (argc > 3) {
		mapFile = argv[1];
		statusFile = argv[2];
		ordersFile = argv[3];
	}
	else {
		// If no command-line arguments are provided, check if default files exist
		if (std::ifstream("mapa.txt")) {
			mapFile = "mapa.txt";
		}
		else {
			std::cout << "No map file specified. Please provide a valid map file.\n";
			return -1;
		}

		if (std::ifstream("status.txt")) {
			statusFile = "status.txt";
		}
		else {
			std::cout << "No status file specified. Please provide a valid status file.\n";
			return -1;
		}

		if (std::ifstream("rozkazy.txt")) {
			ordersFile = "rozkazy.txt";
		}
		else {
			std::cout << "No orders file specified. Please provide a valid orders file.\n";
			return -1;
		}
	}

	std::vector<std::vector<char>> mapArray;
	int rows = 0, columns = 0;
	readMapFromFile(mapFile, mapArray, rows, columns);

	Actors actors;
	OrdersAndAI orders;

	long gold;
	std::vector<Base> bases;
	std::vector<ActiveUnit> activeUnits;
	char playerBaseID;

	readStatusFromFile(statusFile, gold, bases, activeUnits, playerBaseID);

	orders.orderBase(activeUnits, gold, playerBaseID, actors.getUnitStats());
	orders.actionUnits(activeUnits, mapArray, actors.getUnitStats(), bases, actors.getDamageType(), actors.getDamageValue());
	orders.writeOrdersToFile(ordersFile);

	return 0;
}

