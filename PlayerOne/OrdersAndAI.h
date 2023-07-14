#ifndef ORDERSANDAI_H
#define ORDERSANDAI_H

#include "Actors.h"

enum class ActionType {
	Move,
	Attack,
	Produce
};

struct Command {
	ActionType action;
	int unitID;
	int finalCol;
	int finalRow;
	int enemyID;
	int baseID;
	char unitType;
};

class OrdersAndAI {
private:
	std::vector<Command> executedCommands; // Store the executed commands

public:
	OrdersAndAI();

	void orderBase(std::vector<ActiveUnit>& activeUnits, long& gold, int baseID, const std::vector<UnitStats>& unitStats);
	void actionUnits(std::vector<ActiveUnit>& activeUnits, std::vector<std::vector<char>>& mapArray,
	const std::vector<UnitStats>& unitStats, const std::vector<Base>& bases, const std::vector<char>& damageType, const std::vector<std::vector<int>>& damageValue);
	void writeOrdersToFile(const std::string& ordersFile);
};

#endif
