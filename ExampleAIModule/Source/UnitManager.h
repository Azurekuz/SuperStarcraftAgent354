#pragma once
#include <forward_list>
#include <queue>
#include <BWAPI.h>
//#include "regionNode.h"

//#include "../SparCraft/source/SparCraft.h"
class UnitManager {
	std::list<BWAPI::Unit> allCombatUnits;
	std::list<BWAPI::Region> marchPath;

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

	bool testPathFound = false;
	bool isDebug = true;

public:
	UnitManager();
	virtual void commandUnits();
	//virtual void marchToward(BWAPI::Region start, BWAPI::Region destination);
	//virtual void genShortPath(BWAPI::Region curPos, BWAPI::Region start, std::map<BWAPI::Region, regionNode> visitedFrom);
	virtual void addUnit(BWAPI::Unit newUnit);
	virtual bool sortUnit(BWAPI::Unit newUnit);
	virtual bool removeUnit(BWAPI::Unit unit);
	virtual void UnitManager::retaliate(BWAPI::Position destroyed);
};