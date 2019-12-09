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
	visitedRegions.push_front(Broodwar->getRegionAt(Position(Broodwar->self()->getStartLocation())));
	currentDestination = visitedRegions.front();

	Broodwar << Position((Broodwar->enemy()->getStartLocation())) << std::endl;
	genMarchPath(Broodwar->getRegionAt(Position(Broodwar->self()->getStartLocation())), Broodwar->getRegionAt(Position(1000, 1002)));
	Broodwar << marchPath.size() << std::endl;

	//Position((Broodwar->enemy()->getStartLocation())))
}

void UnitManager::commandUnits() {
	searchRegions();
	for (BWAPI::Unit &u : allCombatUnits) {
		checkUnitRegion(u);
		if (u->isIdle()) {
			if(allCombatUnits.size() < 5){
				u->patrol(u->getRegion()->getClosestAccessibleRegion()->getCenter());
			}
			else {
				navigateUnit(u);
				/*if (std::find(visitedRegions.begin(), visitedRegions.end(), currentDestination) == visitedRegions.end()) {
					while (!toVisit.empty() || std::find(visitedRegions.begin(), visitedRegions.end(), currentDestination) == visitedRegions.end()) {
						if (toVisit.front()->isAccessible()) {
							currentDestination = toVisit.front();
						}
						toVisit.pop();
					}
				}*/
				//BWAPI::Position destination = chooseRegion(allUnitSet.getPosition())->getCenter() + Position((std::rand() * 32) - 16, (std::rand() * 32) - 16);
				//marchToward(destination);
			}
		}
		
	}
}

void UnitManager::navigateUnit(BWAPI::Unit unit) {
	//unit->move(toVisit.front()->getCenter());
}

void UnitManager::genMarchPath(BWAPI::Region start, BWAPI::Region destination) {
	std::map <BWAPI::Region, regionNode> visitedFrom;
	marchPath.clear();

	toVisit.push(regionNode(start, start->getDistance(destination), 0));
	BWAPI::Region currentRegion = start;
	while (!(toVisit.empty())) {
		regionNode curNode = toVisit.front();
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

BWAPI::Region UnitManager::chooseRegion(BWAPI::Position position) {
	BWAPI::Regionset neighbors = Broodwar->getRegionAt(allUnitSet.getPosition())->getNeighbors();
	BWAPI::Region currentRegion;
	int regionVal = -999;
	for (const BWAPI::Region &r: neighbors) {
		int currentVal = 0;
		if (regionVal == -999) {
			currentRegion = r;
		}
		
		if (std::find(visitedRegions.begin(), visitedRegions.end(), r) == visitedRegions.end()) {
			currentVal = currentVal + 2;
		}
		else {
			currentVal = currentVal - 2;
		}

		/*if (!r->isAccessible()) {
			currentVal = currentVal - 1;
		}*/

		if (regionVal < currentVal) {
			currentRegion = r;
			regionVal = currentVal;
		}
		else if (regionVal == currentVal) {
			int roll = std::rand() * 1;
			if (roll == 0) {
				currentRegion = r;
				regionVal = currentVal;
			}
		}
	}
	return currentRegion;
}

void UnitManager::checkUnitRegion(BWAPI::Unit unit) {
	if (std::find(visitedRegions.begin(), visitedRegions.end(), unit->getRegion()) == visitedRegions.end()) {
		visitedRegions.push_front(unit->getRegion());
		Broodwar << "New Region added!" << std::endl;
	}
}

void UnitManager::checkBaseLocations() {
	for (const BWAPI::Player &p :Broodwar->getPlayers()) {
		if (p->isEnemy(Broodwar->self()) && std::find(visitedRegions.begin(), visitedRegions.end(), Broodwar->getRegionAt(Position(p->getStartLocation()))) != visitedRegions.end()){
			marchToward(Position(p->getStartLocation()));
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
	/*for (BWAPI::Unit &u : allCombatUnits) {
		if (u->isIdle() || u->isPatrolling()) {
			u->attack(target);
		}
	}*/
	allUnitSet.attack(target);
}

void UnitManager::searchRegions() {
	for(const BWAPI::Region &vr : visitedRegions){
		for (const BWAPI::Region &r: vr->getNeighbors()) {
			if (std::find(visitedRegions.begin(), visitedRegions.end(), r) == visitedRegions.end()) {
				//toVisit.push(r);
			}
		}
	}
}

void UnitManager::retaliate(BWAPI::Position destroyed) {
	for (BWAPI::Unit &u : allCombatUnits) {
		u->patrol(destroyed);
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