#ifndef ACTORS_H
#define ACTORS_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

struct UnitStats {
	char type;
	int endurance;
	int speed;
	int cost;
	int range;
	int buildingTime;
};

struct Base {
	char unitAffiliation;
	char unitType;
	int ID;
	int rowValue;
	int colValue;
	int enduranceLeft;
	int producing;
};

struct ActiveUnit {
	char unitAffiliation;
	char unitType;
	int ID;
	int rowValue;
	int colValue;
	int enduranceLeft;
};

class Actors {
private:
	std::vector<UnitStats> unitStats;
	std::vector<char> damageType;
	std::vector<std::vector<int>> damageValue;

public:
	Actors();

	const std::vector<UnitStats>& getUnitStats() const;
	const std::vector<char>& getDamageType() const;
	const std::vector<std::vector<int>>& getDamageValue() const;
	const UnitStats& getUnitStats(char unitType) const;
	void move(std::vector<std::vector<char>>& mapArray, std::vector<ActiveUnit>& activeUnits, const std::vector<Base>& bases, char direction, int unitID);
	void attack(std::vector<ActiveUnit>& activeUnits, int unitID, int enemyID);
};

#endif
