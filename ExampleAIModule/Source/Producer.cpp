#include "Producer.h"
#include <iostream>
#include <ctime>


using namespace BWAPI;


/* Cobi O'Connell
1) Get base system for troop production (marines, vultures, siege machines, goliaths). 
2) Get base system for research/upgrades (getting siege mode is high priority).
3) Incorporate troop requests from unit manager (1/2 for requests, 1/2 for default, for now).
4) Test default troop combos and research order to see what wins more often.*/


Producer::Producer() {
	Producer::commandcentersList = {};
	Producer::barracksList = {};
	Producer::factoriesList = {};
	Producer::starportsLists = {};
	//Producer::machineshopsList = {}; //for research
	//Producer::armoriesList = {}; //for research
	Producer::armoriesAndMS = {}; //machine shops and armories list
	Producer::otherList = {}; //other buildings not specified
	//Producer::upgradeResearchOrder = {UpgradeTypes::Charon_Boosters, UpgradeTypes::Terran_Vehicle_Weapons, UpgradeTypes::Terran_Vehicle_Plating}; //order queue for upgrade type researching
	//Producer::techResearchOrder = { TechTypes::Tank_Siege_Mode }; //order queue for tech type researching
}

void Producer::trainTroops() {
	trainSCVs();
	trainFactoryTroops();
	trainMarines();
	}

void Producer::trainMarines() {
	//train multiple marines at once if no factories
	if (factoriesList.size() < 1) {
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
	else if (factoriesList.size() > 0 && !barracksList.empty()){
		//only train one marine at once if there is a factory
		BWAPI::Unit xx = barracksList.front();
			if (xx->isIdle()) {
				xx->train(UnitTypes::Terran_Marine);
			}
	}
}

void Producer::trainSCVs() {
	for (BWAPI::Unit& cc : commandcentersList) {
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

void Producer::trainFactoryTroops() {
	int vultureCount = 0;
	for (BWAPI::Unit& f : factoriesList) {
		//If you can make both goliaths and siege tanks, alternate between the two
		if (f->canTrain(UnitTypes::Terran_Goliath) && f->canTrain(UnitTypes::Terran_Siege_Tank_Tank_Mode)) {
			if (f->isIdle()) {
				if (troopFlip == true) {
					f->train(UnitTypes::Terran_Goliath);
					troopFlip = false;
				}
				else {
					f->train(UnitTypes::Terran_Siege_Tank_Tank_Mode);
					troopFlip = true;
				} 
			}
		}
		//if cannot make goliaths but can make siege tanks
		else if (f->canTrain(UnitTypes::Terran_Siege_Tank_Tank_Mode)) {
			if (f->isIdle()) {
				f->train(UnitTypes::Terran_Siege_Tank_Tank_Mode);
			}
		}
		//if cannot make siege tanks or goliaths but do have a basic factory
		else {
			if (f->isIdle() && vultureCount < 2) {
				f->train(UnitTypes::Terran_Vulture);
				vultureCount++;
			}
		}
	}
}

//hard-coded for now for functionality
void Producer::research() {
	for (BWAPI::Unit& rs : armoriesAndMS) {
		if (rs->isIdle()) {
			if (rs->canResearch(TechTypes::Tank_Siege_Mode)) {
				rs->research(TechTypes::Tank_Siege_Mode);
				if (isTest) {
					BWAPI::Broodwar << "Started researching: Tank Siege Mode " << std::endl;
				}
			}
			else if (rs->canUpgrade(UpgradeTypes::Charon_Boosters)) {
				rs->upgrade(UpgradeTypes::Charon_Boosters);
				if (isTest) {
					BWAPI::Broodwar << "Started upgrading: Charon Boosters " << std::endl;
				}
			}
			else if (rs->canUpgrade(UpgradeTypes::Terran_Vehicle_Weapons)) {
				rs->upgrade(UpgradeTypes::Terran_Vehicle_Weapons);
				if (isTest) {
					BWAPI::Broodwar << "Started upgrading: Vehicle Weapons " << std::endl;
				}
			}
			else if (rs->canUpgrade(UpgradeTypes::Terran_Vehicle_Plating)) {
				rs->upgrade(UpgradeTypes::Terran_Vehicle_Plating);
				if (isTest) {
					BWAPI::Broodwar << "Started upgrading: Vehicle Plating " << std::endl;
				}
			}
		}
	}
	/**if (!techResearchOrder.empty()) {
		BWAPI::TechType tt = techResearchOrder.front();
		for (BWAPI::Unit& ms : machineshopsList) {
			if (ms->canResearch(tt)) {
				ms->research(tt);
				techResearchOrder.pop_front();
				if (isTest) {
					BWAPI::Broodwar << "Started researching: " + tt << std::endl;
				}
			}
		}
	} **/
}

void Producer::addBuilding(Unit unit) {
	if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center) {
		Producer::commandcentersList.push_front(unit);
		if (isTest) {
			BWAPI::Broodwar << "Added Command Center to list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks) {
		Producer::barracksList.push_front(unit);
		if (isTest) {
			BWAPI::Broodwar << "Added Barrack to list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Factory) {
		Producer::factoriesList.push_front(unit);
		if (isTest) {
			BWAPI::Broodwar << "Added Factory to list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Starport) {
		Producer::starportsLists.push_front(unit);
		if (isTest) {
			BWAPI::Broodwar << "Added Starport to list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Machine_Shop) {
		//Producer::machineshopsList.push_front(unit);
		Producer::armoriesAndMS.push_front(unit);
		if (isTest) {
			BWAPI::Broodwar << "Added Machine Shop to list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Armory) {
		//Producer::armoriesList.push_front(unit);
		Producer::armoriesAndMS.push_front(unit);
		if (isTest) {
			BWAPI::Broodwar << "Added Armory to list" << std::endl;
		}
	}
	else {
		Producer::otherList.push_front(unit);
		if (isTest) {
			BWAPI::Broodwar << "Added Other to list" << std::endl;
		}
	}
}


void Producer::removeBuilding(Unit unit) {
	if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center) {
		Producer::commandcentersList.remove(unit);
		if (isTest) {
			BWAPI::Broodwar << "Removed Command Center from list" << std::endl;
		}
		
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Barracks) {
		Producer::barracksList.remove(unit);
		if (isTest) {
			BWAPI::Broodwar << "Removed Barracks from list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Factory) {
		Producer::factoriesList.remove(unit);
		if (isTest) {
			BWAPI::Broodwar << "Removed Factory from list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Starport) {
		Producer::starportsLists.remove(unit);
		if (isTest) {
			BWAPI::Broodwar << "Removed Starport from list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Machine_Shop) {
		//Producer::machineshopsList.remove(unit);
		Producer::armoriesAndMS.remove(unit);
		if (isTest) {
			BWAPI::Broodwar << "Removed Machine Shop from list" << std::endl;
		}
	}
	else if (unit->getType() == BWAPI::UnitTypes::Terran_Armory) {
		//Producer::armoriesList.remove(unit);
		Producer::armoriesAndMS.remove(unit);
		if (isTest) {
			BWAPI::Broodwar << "Removed Armory from list" << std::endl;
		}
	} 
	else {
		Producer::otherList.remove(unit);
		if (isTest) {
			BWAPI::Broodwar << "Removed Other from list" << std::endl;
		}
	}
}

