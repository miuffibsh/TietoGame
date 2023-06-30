#include "Actors.h"

Actors::Actors() {
	unitStats = {
		{'K', 70, 5, 400, 1, 5},
		{'S', 60, 2, 250, 1, 3},
		{'A', 40, 2, 250, 5, 3},
		{'P', 50, 2, 200, 2, 3},
		{'R', 90, 2, 500, 1, 4},
		{'C', 50, 2, 800, 7, 6},
		{'W', 20, 2, 100, 1, 2}
	};

	damageType = { 'K', 'S', 'A', 'P', 'C', 'R', 'W', 'B' };

	damageValue = {
		{35, 35, 35, 35, 35, 50, 35, 35},
		{30, 30, 30, 20, 20, 30, 30, 30},
		{15, 15, 15, 15, 10, 10, 15, 15},
		{35, 15, 15, 15, 15, 10, 15, 10},
		{40, 40, 40, 40, 40, 40, 40, 50},
		{10, 10, 10, 10, 10, 10, 10, 50},
		{5, 5, 5, 5, 5, 5, 5, 1}
	};
}

const std::vector<UnitStats>& Actors::getUnitStats() const {
	return unitStats;
}

const std::vector<char>& Actors::getDamageType() const {
	return damageType;
}

const std::vector<std::vector<int>>& Actors::getDamageValue() const {
	return damageValue;
}

const UnitStats& Actors::getUnitStats(char unitType) const {
	for (const auto& unit : unitStats) {
		if (unit.type == unitType) {
			return unit;
		}
	}
	// Return a default UnitStats object if the unit type is not found
	static UnitStats defaultUnit;
	return defaultUnit;
}

void Actors::move(std::vector<std::vector<char>>& mapArray, std::vector<ActiveUnit>& activeUnits, const std::vector<Base>& bases, char direction, int unitID) {
	// Find the unit with the specified ID in the activeUnits array
	for (auto& unit : activeUnits) {
		if (unit.ID == unitID) {
			// Calculate the new position based on the direction
			int newRow = unit.rowValue;
			int newCol = unit.colValue;

			if (direction == 'U') {
				newRow--;
			}
			else if (direction == 'D') {
				newRow++;
			}
			else if (direction == 'L') {
				newCol--;
			}
			else if (direction == 'R') {
				newCol++;
			}

			// Check if the new position is within the map boundaries is not occupied by an obstacle
			if (newRow >= 0 && newRow < mapArray.size() && newCol >= 0 && newCol < mapArray[0].size() && mapArray[newRow][newCol] != '9') {
				// Check if the new position is not occupied by enemy base, or enemy unit
				bool isOccupied = false;
				for (const auto& base : bases) {
					if (base.rowValue == newRow && base.colValue == newCol && base.unitAffiliation == 'E') {
						isOccupied = true;
						break;
					}
				}

				for (const auto& enemyUnit : activeUnits) {
					if (enemyUnit.rowValue == newRow && enemyUnit.colValue == newCol && enemyUnit.unitAffiliation == 'E') {
						isOccupied = true;
						break;
					}
				}

				if (!isOccupied) {
					// Update the position of the unit
					unit.rowValue = newRow;
					unit.colValue = newCol;
				}
			}

			// Exit the loop once the unit is found
			break;
		}
	}
}

void Actors::attack(std::vector<ActiveUnit>& activeUnits, int unitID, int enemyID) {
	// Find the attacking unit with the specified ID
	const ActiveUnit* attacker = nullptr;
	for (const auto& unit : activeUnits) {
		if (unit.ID == unitID) {
			attacker = &unit;
			break;
		}
	}

	// Find the enemy unit with the specified ID
	ActiveUnit* enemy = nullptr;
	for (auto& unit : activeUnits) {
		if (unit.ID == enemyID) {
			enemy = &unit;
			break;
		}
	}

	// Check if both the attacking unit and the enemy unit exist
	if (attacker && enemy) {
		// Calculate the range of the attack
		int rangeOfAttack = std::abs(attacker->colValue - enemy->colValue) + std::abs(attacker->rowValue - enemy->rowValue);

		// Get the max range of the attacking unit from unitStats
		const UnitStats& attackerStats = getUnitStats(attacker->unitType);
		int maxRange = attackerStats.range;

		// Check if the max range is greater than or equal to the range of attack
		if (maxRange >= rangeOfAttack) {
			// Find the damage of the attacking unit's type and enemy unit's type
			char attackerType = attacker->unitType;
			char enemyType = enemy->unitType;

			int attackerTypeIndex = -1;
			int enemyTypeIndex = -1;

			// Find the indices of the attacker type and enemy type in damageType
			for (size_t i = 0; i < damageType.size(); i++) {
				if (damageType[i] == attackerType) {
					attackerTypeIndex = static_cast<int>(i);
				}
				if (damageType[i] == enemyType) {
					enemyTypeIndex = static_cast<int>(i);
				}
			}

			// If both types are found, subtract the damage from the enemy's enduranceLeft
			if (attackerTypeIndex != -1 && enemyTypeIndex != -1) {
				int damage = damageValue[attackerTypeIndex][enemyTypeIndex];
				enemy->enduranceLeft -= damage;
			}
		}
	}
}

