#pragma once
#include <forward_list>
#include <queue>
#include <BWAPI.h>
#include "regionNode.h"
#include <time.h>

class UnitManager {
	std::list<BWAPI::Unit> allCombatUnits;
	BWAPI::Unitset allUnitSet;

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

	std::map <BWAPI::Region, regionNode> visitedFrom;
	std::priority_queue<regionNode, std::vector<regionNode>,std::greater<regionNode>> toVisit;
	std::list<BWAPI::Region> marchPath;
	BWAPI::Region startRegion;
	BWAPI::Region enemyRegion;
	
	clock_t nowTime = clock();
	clock_t lastTime = nowTime;
	regionNode curNode;
	bool isDebug = true;

public:
	UnitManager();
	virtual void commandUnits();
	virtual void UnitManager::marchToward(BWAPI::Position destination);
	virtual void addUnit(BWAPI::Unit newUnit);
	virtual bool sortUnit(BWAPI::Unit newUnit);
	virtual bool removeUnit(BWAPI::Unit unit);
	virtual void UnitManager::retaliate(BWAPI::Position destroyed);
	virtual std::list<BWAPI::Unit> UnitManager::getCombatUnits();
	virtual void UnitManager::allAttack(BWAPI::Unit target);
	virtual void UnitManager::genMarchPath(BWAPI::Region start, BWAPI::Region destination);
	virtual void UnitManager::genShortPath(BWAPI::Region curPos, BWAPI::Region start, std::map<BWAPI::Region, regionNode> visitedFrom);
	void UnitManager::navigateUnit(BWAPI::Unit unit);
	virtual void UnitManager::checkUnexplored(BWAPI::Region curRegion);
};
