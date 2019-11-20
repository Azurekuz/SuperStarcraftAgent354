#include "UnitManager.h"

using namespace BWAPI;

UnitManager::UnitManager() {

}

void UnitManager::commandUnits(){
	for (BWAPI::Unit &u : allCombatUnits) {
		if (u->isIdle() && allCombatUnits.size() < 15){
			u->patrol(u->getRegion()->getClosestAccessibleRegion()->getCenter());
		}
		else if(allCombatUnits.size() >= 15){
			BWAPI::Position enemyLoc = Position(Broodwar->enemy()->getStartLocation());
			if(u ->hasPath(enemyLoc)){
				u->attack(enemyLoc);
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