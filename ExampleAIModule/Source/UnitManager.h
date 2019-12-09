#pragma once
#include <forward_list>
#include <queue>
#include <BWAPI.h>
#include "regionNode.h"
//#include "../SparCraft/source/SparCraft.h"
class UnitManager {
	std::list<BWAPI::Unit> allCombatUnits;
	BWAPI::Unitset allUnitSet;

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

	std::list<BWAPI::Region> visitedRegions;
	std::queue<regionNode> toVisit;
	BWAPI::Region currentDestination;
	bool isDebug = false;

public:
	UnitManager();
	virtual void commandUnits();
	virtual void UnitManager::checkBaseLocations();
	virtual void UnitManager::marchToward(BWAPI::Position destination);
	virtual void addUnit(BWAPI::Unit newUnit);
	virtual bool sortUnit(BWAPI::Unit newUnit);
	virtual bool removeUnit(BWAPI::Unit unit);
	virtual void UnitManager::retaliate(BWAPI::Position destroyed);
	virtual void UnitManager::searchRegions();
	virtual std::list<BWAPI::Unit> UnitManager::getCombatUnits();
	virtual void UnitManager::allAttack(BWAPI::Unit target);
	virtual void UnitManager::checkUnitRegion(BWAPI::Unit unit);
	virtual BWAPI::Region UnitManager::chooseRegion(BWAPI::Position position);
	virtual void UnitManager::genMarchPath(BWAPI::Region start, BWAPI::Region destination);
	virtual void UnitManager::genShortPath(BWAPI::Region curPos, BWAPI::Region start, std::map<BWAPI::Region, regionNode> visitedFrom);
	void UnitManager::navigateUnit(BWAPI::Unit unit);
};
