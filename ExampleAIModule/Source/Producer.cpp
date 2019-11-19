#include "Producer.h"
#include <iostream>
#include <ctime>


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
	Producer::otherList = {}; //other buildings not specified
} 

void Producer::trainTroops() {
	trainMarines();
	trainSCVs();
}

void Producer::trainMarines() {
	for (BWAPI::Unit& x : barracksList) {
		// Order the barracks to train more marines! But only when it is idle.
		if (x->isIdle() && !x->train(UnitTypes::Terran_Marine))
		{
			// If that fails, draw the error at the location so that you can visibly see what went wrong!
			// However, drawing the error once will only appear for a single frame
			// so create an event that keeps it on the screen for some frames
			Position pos = x->getPosition();
			Error lastErr = Broodwar->getLastError();
			Broodwar->registerEvent([pos, lastErr](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
				nullptr,    // condition
				Broodwar->getLatencyFrames());  // frames to run
		} // closure: failed to train idle unit
	}
}

void Producer::trainSCVs() {
	for (Unit cc : commandcentersList) {
		if (cc->isIdle() && !cc->train(UnitTypes::Terran_SCV))
		{
			// If that fails, draw the error at the location so that you can visibly see what went wrong!
			// However, drawing the error once will only appear for a single frame
			// so create an event that keeps it on the screen for some frames
			Position pos = cc->getPosition();
			Error lastErr = Broodwar->getLastError();
			Broodwar->registerEvent([pos, lastErr](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
				nullptr,    // condition
				Broodwar->getLatencyFrames());  // frames to run
		}
	}
}



void Producer::research()
{
	
}

void Producer::addBuilding(Unit unit) {
	if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center) {
		Producer::commandcentersList.push_front(unit);
		BWAPI::Broodwar << "Added Command Center to list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks) {
		Producer::barracksList.push_front(unit);
		BWAPI::Broodwar << "Added Barrack to list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Factory) {
		Producer::factoriesList.push_front(unit);
		BWAPI::Broodwar << "Added Factory to list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Starport) {
		Producer::starportsLists.push_front(unit);
		BWAPI::Broodwar << "Added Starport to list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Machine_Shop) {
		Producer::machineshopsList.push_front(unit);
		BWAPI::Broodwar << "Added Machine Shop to list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Armory) {
		Producer::armoriesList.push_front(unit);
		BWAPI::Broodwar << "Added Armory to list" << std::endl;
	}
	else {
		Producer::otherList.push_front(unit);
		BWAPI::Broodwar << "Added Other to list" << std::endl;
	}
}


void Producer::removeBuilding(Unit unit) {
	if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center) {
		Producer::commandcentersList.remove(unit);
		BWAPI::Broodwar << "Removed Command Center from list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks) {
		Producer::barracksList.remove(unit);
		BWAPI::Broodwar << "Removed Barracks from list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Factory) {
		Producer::factoriesList.remove(unit);
		BWAPI::Broodwar << "Removed Factory from list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Starport) {
		Producer::starportsLists.remove(unit);
		BWAPI::Broodwar << "Removed Starport from list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Machine_Shop) {
		Producer::machineshopsList.remove(unit);
		BWAPI::Broodwar << "Removed Machine Shop from list" << std::endl;
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Armory) {
		Producer::armoriesList.remove(unit);
		BWAPI::Broodwar << "Removed Armory from list" << std::endl;
	}
	else {
		Producer::otherList.remove(unit);
		BWAPI::Broodwar << "Removed Other from list" << std::endl;
	}
}