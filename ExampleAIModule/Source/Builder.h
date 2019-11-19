#pragma once

#include <BWAPI.h>
#include "WorkerManager.h"
class Builder
{
private:
	WorkerManager* workerManager;
	BWAPI::Unit homeBase;
	int lastCheckedSupply = 0;
	int lastCheckedBarracks = 0;

public:
	Builder();
	Builder(WorkerManager* wm, BWAPI::Unit hb);

	void Builder::checkBuild();
	void Builder::build(BWAPI::TilePosition targetBuildLocation, BWAPI::UnitType buildingType);
};