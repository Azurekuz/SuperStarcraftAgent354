#pragma once

#include <BWAPI.h>
#include "WorkerManager.h"
#include "Producer.h"
class Builder
{
private:
	WorkerManager* workerManager;
	BWAPI::Unit homeBase;
	Producer* producer;
	int lastCheckedSupply = 0;
	int lastCheckedBarracks = 0;
	int lastCheckedRefinery = 0; 
	int lastCheckedFactory = 0;
	int lastCheckedMachineShop = 0;

	//Initializing the count number of facilities
	int barracksCount = 0;
	int factoriesCount = 0;

	//Initializng the limit number of facilities
	int barracksLimit = 2;
	int factoriesLimit = 3;

public:
	Builder();
	Builder(WorkerManager* wm, Producer* pdcr, BWAPI::Unit hb);

	void Builder::checkBuild();
	void Builder::build(BWAPI::TilePosition targetBuildLocation, BWAPI::UnitType buildingType);
};