#pragma once
#include <BWAPI.h>
#include <forward_list>
#include "../SparCraft/source/SparCraft.h"

class UnitManager {
	std::forward_list<BWAPI::Unit*> allCombatUnits;

	std::forward_list<BWAPI::Unit> cruiserUnits;
	std::forward_list<BWAPI::Unit> dropshipUnits;
	std::forward_list<BWAPI::Unit> firebatUnits;
	std::forward_list<BWAPI::Unit> ghostUnits;
	std::forward_list<BWAPI::Unit> goliathUnits;
	std::forward_list<BWAPI::Unit> marineUnits;
	std::forward_list<BWAPI::Unit> medicUnits;
	std::forward_list<BWAPI::Unit> vesselUnits;
	std::forward_list<BWAPI::Unit> siegeUnits;
	std::forward_list<BWAPI::Unit> valkyrieUnits;
	std::forward_list<BWAPI::Unit> vultureUnits;

	bool isDebug = true;

public:
	UnitManager();

	virtual bool addUnit(BWAPI::Unit newUnit);
	virtual bool removeUnit(BWAPI::Unit unit);
	//virtual bool isCombatUnit(BWAPI::Unit unit);

};