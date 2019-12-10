#include "Builder.h"
//#include "BWAPI/Interface.cpp"
#include <iostream>

using namespace BWAPI;
using namespace Filter;
/* Desmond Lee
TODO: 
-Build Machine Shop
-Expand():
check for more places to build stuff
*/

Builder::Builder() {

}

Builder::Builder(WorkerManager* wm, Producer* pdcr, BWAPI::Unit hb){
	workerManager = wm;
	homeBase = hb;
	producer = pdcr;
}

void Builder::checkBuild() {
	BWAPI::UnitType buildingType = BWAPI::UnitTypes::Terran_Supply_Depot;
	if (Broodwar->self()->supplyUsed() > Broodwar->self()->supplyTotal() * 8 / 10 && Broodwar->self()->minerals() > 100 && Broodwar->getFrameCount() > lastCheckedSupply + 400) {
		//Broodwar << "Create Supply Depot!" << std::endl;
		lastCheckedSupply = Broodwar->getFrameCount();
		build(Broodwar->getBuildLocation(buildingType, homeBase->getTilePosition()), buildingType);
	}

	//Barracks=150
	buildingType = BWAPI::UnitTypes::Terran_Barracks;
	if (Broodwar->self()->minerals() > 150 && Broodwar->getFrameCount() > lastCheckedBarracks + 400 && barracksCount < barracksLimit) {
		//Broodwar << "Create Barracks!" << std::endl;
		BWAPI::bwout << "Barracks Count:" << std::endl;
		BWAPI::bwout << barracksCount << std::endl;
		lastCheckedBarracks = Broodwar->getFrameCount();
		build(Broodwar->getBuildLocation(buildingType, homeBase->getTilePosition()), buildingType);
	}


	//Refinery=100
	buildingType = BWAPI::UnitTypes::Terran_Refinery;
	if (Broodwar->self()->minerals() > 250 && Broodwar->getFrameCount() > lastCheckedRefinery + 400) {
		//Broodwar << "Create Refinery" << std::endl;
		lastCheckedRefinery = Broodwar->getFrameCount();
		build(Broodwar->getBuildLocation(buildingType, homeBase->getTilePosition()), buildingType);
	}

	//Factory=(200, 100)
	buildingType = BWAPI::UnitTypes::Terran_Factory;
	if (Broodwar->self()->minerals() > 200 && Broodwar->self()->gas() > 100 && Broodwar->getFrameCount() > lastCheckedFactory + 400 && Broodwar->self()->isUnitAvailable(buildingType) && barracksCount >= barracksLimit && factoriesCount < factoriesLimit) {
		//Broodwar << "Create Factory" << std::endl;
		BWAPI::bwout << "Factories Count:" << std::endl;
		BWAPI::bwout << factoriesCount << std::endl;
		lastCheckedFactory = Broodwar->getFrameCount();
		build(Broodwar->getBuildLocation(buildingType, homeBase->getTilePosition()), buildingType);
	}

	buildingType = BWAPI::UnitTypes::Terran_Armory;
	if (Broodwar->self()->minerals() > 100 && Broodwar->self()->gas() > 50 && Broodwar->getFrameCount() > lastCheckedArmory + 400 && Broodwar->self()->isUnitAvailable(buildingType) && barracksCount >= barracksLimit && factoriesCount >= factoriesLimit && armoryCount < armoryLimit) {
		//Broodwar << "Create Armory" << std::endl;
		lastCheckedArmory = Broodwar->getFrameCount();
		build(Broodwar->getBuildLocation(buildingType, homeBase->getTilePosition()), buildingType);
	}

	if (factoriesList.size()>0) {
		factoriesList = producer->factoriesList;
		Unit factory = factoriesList.front();

		//Machine Shop(50,50)
		buildingType = BWAPI::UnitTypes::Terran_Machine_Shop;
		if (Broodwar->self()->minerals() > 50 && Broodwar->self()->gas() > 50 && Broodwar->getFrameCount() > lastCheckedMachineShop + 400 && Broodwar->self()->isUnitAvailable(buildingType) && factory != nullptr && factory->canBuildAddon(buildingType)) {
			//Broodwar << "Create Machine Shop" << std::endl;
			lastCheckedMachineShop = Broodwar->getFrameCount();
			factory->buildAddon(BWAPI::UnitTypes::Terran_Machine_Shop);





		}
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

