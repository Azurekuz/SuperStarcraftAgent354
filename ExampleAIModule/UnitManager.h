#pragma once
#include <BWAPI.h>
#include <forward_list>
#include "../SparCraft/source/SparCraft.h"

class UnitManager {
	std::forward_list<BWAPI::Unit> armyUnits;

public:
	virtual void addToMilitary(BWAPI::Unit newUnit);
};