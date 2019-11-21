#include "Builder.h"
//#include "BWAPI/Interface.cpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;
/* Desmond Lee
TODO: 
-checkSupply()
-BuildProduction()
-Expand()
*/

Builder::Builder() {

}

Builder::Builder(WorkerManager* wm, BWAPI::Unit hb){
	workerManager = wm;
	homeBase = hb;
}

void Builder::checkBuild() {
	if (Broodwar->self()->supplyUsed() > Broodwar->self()->supplyTotal() * 8 / 10 && Broodwar->self()->minerals() > 100 && Broodwar->getFrameCount() > lastCheckedSupply + 400) {
		Broodwar << "Create Supply Depot!" << std::endl;
		lastCheckedSupply = Broodwar->getFrameCount();
		BWAPI::UnitType buildingType = BWAPI::UnitTypes::Terran_Supply_Depot;
		build(Broodwar->getBuildLocation(buildingType, homeBase->getTilePosition()), buildingType);
	}

	if (Broodwar->self()->minerals() > 150 && Broodwar->getFrameCount() > lastCheckedBarracks + 400) {
		//Broodwar << "Create Barracks!" << std::endl;
		lastCheckedBarracks = Broodwar->getFrameCount();
		BWAPI::UnitType buildingType = BWAPI::UnitTypes::Terran_Barracks;
		build(Broodwar->getBuildLocation(buildingType, homeBase->getTilePosition()), buildingType);
	}
}



void Builder::build(TilePosition targetBuildLocation, UnitType buildingType){
	Unit builder = workerManager->getBuilder(targetBuildLocation);
	if (targetBuildLocation && builder != nullptr && builder->exists())
	{
		// Register an event that draws the target build location
		Broodwar->registerEvent([targetBuildLocation, buildingType](Game*)
		{
			Broodwar->drawBoxMap(Position(targetBuildLocation),
				Position(targetBuildLocation + buildingType.tileSize()),
				Colors::Blue);
		},
			nullptr,  // condition
			buildingType.buildTime() + 100);  // frames to run

													// Order the builder to construct the supply structure
		builder->build(buildingType, targetBuildLocation);
	}
}