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
	Producer::otherList = {}; //other buildings not specified
} 

void Producer::produceTroops(Unit u)
{
	if (u->getType() == UnitTypes::Terran_Barracks) // checks for terran barricks
		{

		// Order the barracks to train more workers! But only when it is idle.
		if (u->isIdle() && !u->train(UnitTypes::Terran_Marine))
		{
			// If that fails, draw the error at the location so that you can visibly see what went wrong!
			// However, drawing the error once will only appear for a single frame
			// so create an event that keeps it on the screen for some frames
			Position pos = u->getPosition();
			Error lastErr = Broodwar->getLastError();
			Broodwar->registerEvent([pos, lastErr](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, lastErr.c_str()); },   // action
				nullptr,    // condition
				Broodwar->getLatencyFrames());  // frames to run

// to remove when builder can check for supply 
// Retrieve the supply provider type in the case that we have run out of supplies
			UnitType supplyProviderType = u->getType().getRace().getSupplyProvider();
			static int lastCheckedSupply = 0;

			// If we are supply blocked and haven't tried constructing more recently
			if (lastErr == Errors::Insufficient_Supply &&
				lastCheckedSupply + 400 < Broodwar->getFrameCount() &&
				Broodwar->self()->incompleteUnitCount(supplyProviderType) == 0)
			{
				lastCheckedSupply = Broodwar->getFrameCount();

				// Retrieve a unit that is capable of constructing the supply needed
				Unit supplyBuilder = u->getClosestUnit(Filter::GetType == supplyProviderType.whatBuilds().first &&
					(Filter::IsIdle || Filter::IsGatheringMinerals) &&
					Filter::IsOwned);
				// If a unit was found
				if (supplyBuilder)
				{
					buildSupply(supplyBuilder, supplyProviderType);
				} // closure: supplyBuilder is valid
			} // closure: insufficient supply
		} // closure: failed to train idle unit

		}

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

//remove once builder checks supply
void Producer::buildSupply(Unit supplyBuilder, UnitType supplyProviderType) {
	if (supplyProviderType.isBuilding())
	{
		TilePosition targetBuildLocation = Broodwar->getBuildLocation(supplyProviderType, supplyBuilder->getTilePosition());
		if (targetBuildLocation)
		{
			// Register an event that draws the target build location
			Broodwar->registerEvent([targetBuildLocation, supplyProviderType](Game*)
			{
				Broodwar->drawBoxMap(Position(targetBuildLocation),
					Position(targetBuildLocation + supplyProviderType.tileSize()),
					Colors::Blue);
			},
				nullptr,  // condition
				supplyProviderType.buildTime() + 100);  // frames to run

														// Order the builder to construct the supply structure
			supplyBuilder->build(supplyProviderType, targetBuildLocation);
		}
	}
	else
	{
		// Train the supply provider (Overlord) if the provider is not a structure
		supplyBuilder->train(supplyProviderType);
	}
}