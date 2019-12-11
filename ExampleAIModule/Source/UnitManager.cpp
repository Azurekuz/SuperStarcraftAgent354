#include "UnitManager.h"

/**
* Eugene Kuznetsov
* Unit Manager Module
*
* Current Task:
* - Pathfinding to Enemy Base
*
* To Do
* - Organize units into different squads/sets
* - Iterate through and command the sets rather than each individual unit.
**/

using namespace BWAPI;

UnitManager::UnitManager() {
	startRegion = Broodwar->getRegionAt(Position(Broodwar->self()->getStartLocation()));
	enemyRegion = Broodwar->getRegionAt(Position(Broodwar->enemy()->getStartLocation()));
	
	visitedFrom[startRegion] = regionNode(startRegion, (startRegion->getDistance(startRegion)), 0);
	checkUnexplored(startRegion);
	if (!toVisit.empty()) {
		curNode = toVisit.top();
		toVisit.pop();
	}
	nowTime = clock();
	lastTime = nowTime;
}

void UnitManager::commandUnits() {
	for (BWAPI::Unit &u : allCombatUnits) {
		nowTime = clock();
		/*if (u->isIdle()) {
			u->patrol(u->getRegion()->getClosestAccessibleRegion()->getCenter());
		}
		else {
			navigateUnit(u);
		}*/
		navigateUnit(u);
	}
}

void UnitManager::navigateUnit(BWAPI::Unit unit) {
	if(unit->getRegion()!=curNode.getRegion()){
		unit->patrol(curNode.getRegion()->getCenter());
	}
	else if (unit->getRegion() == curNode.getRegion()) {
		checkUnexplored(curNode.getRegion());
		if (!toVisit.empty()) {
			curNode = toVisit.top();
			toVisit.pop();
		}
		nowTime = clock();
		lastTime = nowTime;
	}
	if(difftime(nowTime, lastTime) > 1500 || !unit->isMoving() || !curNode.getRegion()->isAccessible() || !unit->hasPath(curNode.getRegion()->getCenter())){
		if (!toVisit.empty()) {
			curNode = toVisit.top();
			toVisit.pop();
		}
		nowTime = clock();
		lastTime = nowTime;
	}
	//Broodwar << difftime(nowTime, lastTime) << std::endl;
}

void UnitManager::checkUnexplored(BWAPI::Region curRegion) {
	for (const BWAPI::Region &neighbor : curRegion->getNeighbors()) {
		if (visitedFrom.find(neighbor) == visitedFrom.end()) {
			toVisit.push(regionNode(neighbor, neighbor->getDistance(startRegion), visitedFrom[curRegion].getSteps()+1));
		}
	}
}

void UnitManager::genMarchPath(BWAPI::Region start, BWAPI::Region destination) {
	//marchPath.clear();
	//std::map <BWAPI::Region, regionNode> visitedFrom;
	//std::queue<regionNode> toVisit;

	toVisit.push(regionNode(start, start->getDistance(destination), 0));
	BWAPI::Region currentRegion = start;
	while (!(toVisit.empty())) {
		regionNode curNode = toVisit.top();
		toVisit.pop();
		BWAPI::Regionset neighbors = currentRegion->getNeighbors();
		for (BWAPI::Region neighbor : neighbors) {
			if (neighbor == destination) {
				Broodwar << "Destination Found" << std::endl;
				genShortPath(neighbor, start, visitedFrom);
			}
			else if (visitedFrom.find(neighbor) == visitedFrom.end()) {
				toVisit.push(regionNode(neighbor, currentRegion->getDistance(destination), curNode.getSteps()+1));
				visitedFrom[neighbor] = curNode;
			}
		}
	}
}

bool operator<(const regionNode &a, const regionNode &b) {
	return a.nodePriority < b.nodePriority;
}

bool operator>(const regionNode &a, const regionNode &b) {
	return a.nodePriority > b.nodePriority;
}

void UnitManager::genShortPath(BWAPI::Region curPos, BWAPI::Region start, std::map<BWAPI::Region, regionNode> visitedFrom) {
	int curCost = -9999;
	int newCost = -8888;
	while (curPos != start) {
		curCost = NULL;
		marchPath.push_front(curPos);
		BWAPI::Regionset neighbors = curPos->getNeighbors();
		for (BWAPI::Region position : neighbors) {
			if (visitedFrom.find(position) != visitedFrom.end()) {
				regionNode newNode = visitedFrom[position];
				newCost = newNode.getSteps();
				if (curCost == -9999 || newCost < curCost) {
					curCost = newCost;
					curPos = newNode.getRegion();
				}
				else if (curCost == newCost) {
					if (newNode.getRegion()->getDistance(start) < curPos->getDistance(start)) {
						curCost = newCost;
						curPos = newNode.getRegion();
					}
				}
			}
		}
	}
}

void UnitManager::marchToward(BWAPI::Position destination) {
	for (BWAPI::Unit &u : allCombatUnits) {
		if (u->isIdle() || u->isPatrolling() || !(u->isMoving())) {
			u->move(Position(destination));
		}
	}
}

void UnitManager::allAttack(BWAPI::Unit target) {
	allUnitSet.attack(target);
}

void UnitManager::retaliate(BWAPI::Position destroyed) {
	for (BWAPI::Unit &u : allCombatUnits) {
		u->attack(destroyed);
	}
}

void UnitManager::addUnit(BWAPI::Unit newUnit) {
	allCombatUnits.push_front(newUnit);
	allUnitSet.insert(newUnit);
}

bool UnitManager::sortUnit(BWAPI::Unit newUnit)
{
	if (newUnit->getType() == BWAPI::UnitTypes::Terran_Battlecruiser) {
		cruiserUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added BattleCruiser Unit!" << std::endl;
		}
		return true;
	}else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Dropship) {
		dropshipUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Dropship Unit!" << std::endl;
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Firebat) {
		firebatUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Firebat Unit!" << std::endl;
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Ghost) {
		ghostUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Ghost Unit!" << std::endl;
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Goliath) {
		goliathUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Goliath Unit!" << std::endl;
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Marine) {
		marineUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Marine Unit!" << std::endl;
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Medic) {
		medicUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Medic Unit!" << std::endl;
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel) {
		vesselUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Science Vessel Unit!" << std::endl;
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode || newUnit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) {
		siegeUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Siege Tank Unit!" << std::endl;
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Valkyrie) {
		valkyrieUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Valkyrie Unit!" << std::endl;
		}
		return true;
	}else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Vulture) {
		vultureUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Vulture Unit!" << std::endl;
		}
		return true;
	}
	return false;
}

bool UnitManager::removeUnit(BWAPI::Unit unit)
{
	allCombatUnits.remove(unit);
	if (unit->getType() == BWAPI::UnitTypes::Terran_Battlecruiser) {
		cruiserUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed BattleCruiser Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Dropship) {
		dropshipUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Dropship Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Firebat) {
		firebatUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Firebat Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Ghost) {
		ghostUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Ghost Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Goliath) {
		goliathUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Goliath Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Marine) {
		marineUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Marine Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Medic) {
		medicUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Medic Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel) {
		vesselUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Science Vessel Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode || unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) {
		siegeUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Siege Tank Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Valkyrie) {
		valkyrieUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Valkyrie Unit!" << std::endl;
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Vulture) {
		vultureUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Vulture Unit!" << std::endl;
		}
		return true;
	}
	return false;
}

std::list<BWAPI::Unit> UnitManager::getCombatUnits() {
	return allCombatUnits;
}