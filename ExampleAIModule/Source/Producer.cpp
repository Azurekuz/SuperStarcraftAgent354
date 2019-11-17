#include "Producer.h"
#include <iostream>

using namespace BWAPI;

/* Cobi O'Connell
on every frame need to produce and research.
1) Get base system for troop production (marines, vultures, siege machines, goliaths).
2) Get base system for research/upgrades (getting siege mode is high priority).
3) Incorporate troop requests from unit manager (1/2 for requests, 1/2 for default, for now).
4) Test default troop combos and research order to see what wins more often.*/

/* Going to need many more functions, this is just to start*/

Producer::Producer() {
	Producer::commandcentersList = {};
	Producer::barracksList = {};
	Producer::factoriesList = {};
	Producer::starportsLists = {};
	Producer::machineshopsList = {}; //for research
	Producer::armoriesList = {}; //for research
	Producer::otherList = {};
} 

void Producer::produceTroops()
{

}

void Producer::research()
{
	
}

void Producer::addBuilding(Unit unit) {
	if (unit->getType() == UnitTypes::Terran_Command_Center) {
		Producer::commandcentersList.push_front(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Barracks) {
		Producer::barracksList.push_front(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Factory) {
		Producer::factoriesList.push_front(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Starport) {
		Producer::starportsLists.push_front(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Machine_Shop) {
		Producer::machineshopsList.push_front(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Armory) {
		Producer::armoriesList.push_front(unit);
	}
	else {
		Producer::otherList.push_front(unit);
	}
}


void Producer::removeBuilding(Unit unit) {
	if (unit->getType() == UnitTypes::Terran_Command_Center) {
		Producer::commandcentersList.remove(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Barracks) {
		Producer::barracksList.remove(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Factory) {
		Producer::factoriesList.remove(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Starport) {
		Producer::starportsLists.remove(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Machine_Shop) {
		Producer::machineshopsList.remove(unit);
	}
	else if (unit->getType() == UnitTypes::Terran_Armory) {
		Producer::armoriesList.remove(unit);
	}
	else {
		Producer::otherList.remove(unit);
	}
}