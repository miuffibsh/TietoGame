#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <sstream>
#include <tuple>
#include <algorithm>

struct PlayerProgram {
	std::string name;
	std::string mapFile;
	std::string statusFile;
	std::string ordersFile;
	double timeLimit;

	PlayerProgram(const std::string& name, const std::string& mapFile, const std::string& statusFile,
		const std::string& ordersFile, double timeLimit) : name(name), mapFile(mapFile), statusFile(statusFile),
		ordersFile(ordersFile), timeLimit(timeLimit) {}
};

class Mediator {
public:
	Mediator();
	void startGame(const std::string& programName, const std::string& programNameE, 
		int maxGameTurns, double maxTimeLimit, double maxTimeLimitE, std::vector<std::vector<char>> map,
		const std::string& statusFile, const std::string& ordersFile);

private:
	std::vector<PlayerProgram> players;
	std::vector<std::vector<char>> map;
	std::vector<std::string> status;
	int maxGameTurns;
	int maxTimeLimit;
	bool isGameOver();
};

#endif
