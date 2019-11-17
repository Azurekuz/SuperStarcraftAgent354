#include "WorkerManager.h"
#include <iostream>

using namespace BWAPI;

void WorkerManager::manageWorkers() {
	/*manageBuildWorkers();
	assignIdleWorkers();
	manageRepairWorkers();*/

	for (auto &w : idleWorkerList)
	{
		// if our worker is idle
		if (w->isIdle())
		{
			// Order workers carrying a resource to return them to the center,
			// otherwise find a mineral patch to harvest.
			if (w->isCarryingGas() || w->isCarryingMinerals())
			{
				w->returnCargo();
			}
			else if (!w->getPowerUp())  // The worker cannot harvest anything if it
			{                             // is carrying a powerup such as a flag
										  // Harvest from the nearest mineral patch or gas refinery
				if (!w->gather(w->getClosestUnit(Filter::IsMineralField || Filter::IsRefinery)))
				{
					// If the call fails, then print the last error message
					Broodwar << Broodwar->getLastError() << std::endl;
				}

			} // closure: has no powerup
		} // closure: if idle
	}
}

void WorkerManager::manageBuildWorkers() {
	for (auto &w : WorkerManager::buildWorkerList)
	{
		if (!w->exists())
			continue;
		// if our worker is idle
		if (w->isIdle())
		{
			WorkerManager::idleWorkerList.push_front(w);
			WorkerManager::buildWorkerList.remove(w);
		}
	}
}

void WorkerManager::assignIdleWorkers() {
	//Broodwar->sendText("Managing idle workers");
	for (auto &w : idleWorkerList)
	{
		Unit nearestBase = w->getClosestUnit(Filter::IsResourceDepot);
		//Broodwar->sendText("Working with SCV");

		auto itr = WorkerManager::ccToID.find(nearestBase);  //Get ID based on nearest base
		auto itr2 = WorkerManager::resourceMap.find(itr->second);  //Get list of resources at that base from resource map

		std::list<std::array<BWAPI::Unit, 4>> tempList = itr2->second;

		//Broodwar << "Mineral Cluster: " << itr->second << "!" << std::endl;


		auto last = tempList.back();  //Get an iterator to the last value in the list

		int targetIndex = -1;  //Set the target index to invalid

		for (int i = 1; i < 4; i++) {
			if (last.at(i) == nullptr) {
				targetIndex = i;
			}
		}

		for (auto &ar : tempList)
		{
			Broodwar->sendText("Checking tempList");
			if (ar.at(targetIndex) == nullptr) {
				ar.at(targetIndex) = w;
				if (ar.at(0)->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) {
					WorkerManager::mineralWorkerList.push_front({ w, ar.at(0) });
				}
				else {
					WorkerManager::gasWorkerList.push_front({ w, ar.at(0) });
				}

				w->gather(ar.at(0));
				Broodwar->sendText("Sending Mining!");
				WorkerManager::idleWorkerList.remove(w);
			}
		}
	}

	WorkerManager::idleWorkerList.clear();
}

void WorkerManager::manageRepairWorkers() {
	
}

void WorkerManager::addWorker(Unit unit) {
	//Broodwar->sendText("Adding SCV");
	WorkerManager::idleWorkerList.push_front(unit);
}

void WorkerManager::removeWorker(Unit unit) {
	WorkerManager::idleWorkerList.remove(unit);
	WorkerManager::repairWorkerList.remove(unit);
	WorkerManager::mineralWorkerList.remove_if([unit](std::array<BWAPI::Unit, 2> x) { return x.at(0) == unit; });
	WorkerManager::gasWorkerList.remove_if([unit](std::array<BWAPI::Unit, 2> x) { return x.at(0) == unit; });
}

void WorkerManager::addResource(Unit resource) {
	//Broodwar->sendText("Adding a Mineral patch");
	int resourceGroup = resource->getResourceGroup();
	try {
		auto it = WorkerManager::resourceMap.at(resourceGroup);
	}
	catch (std::out_of_range) {
		std::list<std::array<BWAPI::Unit, 4>>* tempList = new std::list<std::array<BWAPI::Unit, 4>>;
		tempList->assign({});
		WorkerManager::resourceMap.insert({ resourceGroup, *tempList });
		/*if (resourceGroup == 2) {
			Broodwar << "New mineral cluster added: " << resourceGroup << "!" << std::endl;
		}*/
	}
	auto tempList = WorkerManager::resourceMap.at(resourceGroup);
	std::array<BWAPI::Unit, 4>* tempArray = new std::array<BWAPI::Unit, 4>{resource, nullptr, nullptr, nullptr};
	tempList.push_front(*tempArray);

	/*if (resourceGroup == 2) {
		Broodwar << "Drawing box!" << std::endl;
		BWAPI::TilePosition pos = resource->getTilePosition();
		BWAPI::UnitType min = BWAPI::UnitTypes::Resource_Mineral_Field;
		Broodwar->registerEvent([pos, min](Game*)
		{
			Broodwar->drawBoxMap(Position(pos),
				Position(pos + min.tileSize()),
				Colors::Green);
		},
			nullptr,  // condition
			10000);  // frames to run
	}*/
}

void WorkerManager::addCC(Unit cc) {
	//Broodwar->sendText("Adding CC");
	Unit nearestMinerals = cc->getClosestUnit(Filter::IsMineralField);
	if (nearestMinerals) {
		WorkerManager::ccToID.insert({ cc, nearestMinerals->getResourceGroup() });
	}
	auto it = WorkerManager::ccToID.find(cc);
	//Broodwar << "Mineral Cluster: " << it->second << "!" << std::endl;
}

BWAPI::Unit WorkerManager::getBuilder(TilePosition loc) {
	return nullptr;
}