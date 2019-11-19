#pragma once

#include <forward_list>
#include <list>
#include <array>
#include <map>
#include <iterator>
#include <BWAPI.h>
class Builder
{
private:
	std::list<BWAPI::Unit> buildWorkerList{};
	std::forward_list<std::array<BWAPI::Unit, 2>> mineralWorkerList{};
	int cummulativeSupply;
	int usedSupply;
	int currentSupply;
	Builder(BWAPI::Race race);
	//Desmond, I was thinking about the best way to do things, and I think our best bet is to get the location we want to build at first, and then pass that as an argument
	//to getBuilder, and I can then get the closest idle worker to build it.  -Orion
	void Builder::checkSupply(BWAPI::Race race);
	void Builder::buildBarracks(BWAPI::UnitType Terran_Barricks);
	void Builder::buildSupply(BWAPI::Unit supplyBuilder, BWAPI::UnitType supplyBuilderType);
};