#include "UnitManager.h"

UnitManager::UnitManager() {

}

void UnitManager::commandUnits()
{
}

bool UnitManager::addUnit(BWAPI::Unit newUnit)
{
	if (newUnit->getType() == BWAPI::UnitTypes::Terran_Battlecruiser) {
		cruiserUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added BattleCruiser Unit!";
		}
		return true;
	}else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Dropship) {
		dropshipUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Dropship Unit!";
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Firebat) {
		firebatUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Firebat Unit!";
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Ghost) {
		ghostUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Ghost Unit!";
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Goliath) {
		goliathUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Goliath Unit!";
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Marine) {
		marineUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Marine Unit!";
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Medic) {
		medicUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Medic Unit!";
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel) {
		vesselUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Science Vessel Unit!";
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode || newUnit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) {
		siegeUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Siege Tank Unit!";
		}
		return true;
	}
	else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Valkyrie) {
		valkyrieUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Valkyrie Unit!";
		}
		return true;
	}else if (newUnit->getType() == BWAPI::UnitTypes::Terran_Vulture) {
		vultureUnits.push_front(newUnit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Added Vulture Unit!";
		}
		return true;
	}

	return false;
}

bool UnitManager::removeUnit(BWAPI::Unit unit)
{
	if (unit->getType() == BWAPI::UnitTypes::Terran_Battlecruiser) {
		cruiserUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed BattleCruiser Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Dropship) {
		dropshipUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Dropship Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Firebat) {
		firebatUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Firebat Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Ghost) {
		ghostUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Ghost Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Goliath) {
		goliathUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Goliath Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Marine) {
		marineUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Marine Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Medic) {
		medicUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Medic Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel) {
		vesselUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Science Vessel Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode || unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) {
		siegeUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Siege Tank Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Valkyrie) {
		valkyrieUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Valkyrie Unit!";
		}
		return true;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Vulture) {
		vultureUnits.remove(unit);
		if (isDebug) {
			BWAPI::Broodwar << "UM: Removed Vulture Unit!";
		}
		return true;
	}

	return false;
}