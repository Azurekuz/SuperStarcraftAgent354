#include "WorkerManager.h"
#include <iostream>

using namespace BWAPI;

void WorkerManager::manageWorkers() {
	manageBuildWorkers();
	assignIdleWorkers();
	manageRepairWorkers();
	manageMineralWorkers();
	manageGasWorkers();

	/*for (auto &w : idleWorkerList)
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
	}*/
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
	for (BWAPI::Unit& w : WorkerManager::idleWorkerList)
	{
		Unit nearestBase = w->getClosestUnit(Filter::IsResourceDepot);
		//Broodwar->sendText("Working with SCV");

		std::list<std::array<BWAPI::Unit, 4>> cluster = WorkerManager::resourceMap[WorkerManager::ccToID[nearestBase]];  //Get list of resources at that base from resource map

		//Broodwar << "Mineral Cluster: " << WorkerManager::ccToID[nearestBase] << "!" << std::endl;


		auto last = cluster.back();  //Get an iterator to the last value in the list

		int targetIndex = -1;  //Set the target index to invalid

		for (int i = 1; i < 4; i++) {
			if (last[i] == nullptr) {
				targetIndex = i;
				break;
			}
		}

		for (auto &ar : cluster)
		{
			if (ar[targetIndex] == nullptr) {
				//Broodwar << "Targetindex: " << targetIndex << "!" << std::endl;
				//Broodwar << "SCV assigned to patch: " << ar[0] << "!" << std::endl;
				ar[targetIndex] = w;
				//Broodwar << "SCV address: " << ar[targetIndex] << "!" << std::endl;
				if (ar[0]->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) {
					WorkerManager::mineralWorkerList.push_front({ w, ar[0] });
				}
				else {
					WorkerManager::gasWorkerList.push_front({ w, ar[0] });
				}
				//WorkerManager::idleWorkerList.remove(w);

				break;
			}
		}
		WorkerManager::resourceMap[WorkerManager::ccToID[nearestBase]] = cluster;
	}

	idleWorkerList.clear();
}

void WorkerManager::manageRepairWorkers() {
	
}

void WorkerManager::manageMineralWorkers() {
	for (auto& a : WorkerManager::mineralWorkerList) {
		if (a[0]->isIdle())
		{
			if (a[0]->isCarryingMinerals()) {
				//Broodwar->sendText("Carrying Minerals, return it!");
				a[0]->returnCargo();
			}
			else if (!a[0]->isGatheringMinerals()) {
				//Broodwar << "Not Gathering, gather from: " << a[1] << "!" << std::endl;
				a[0]->gather(a[1]);
			}
		}
	}

}

void WorkerManager::manageGasWorkers() {
	for (auto& a : WorkerManager::gasWorkerList) {
		if (a[0]->isIdle()) {
			if (a[0]->isCarryingGas()) {
				a[0]->returnCargo();
			}
			else {
				a[0]->gather(a[1]);
			}
		}
	}
}

void WorkerManager::addWorker(Unit unit) {
	if (unit != nullptr) {
		//Broodwar->sendText("Adding SCV");
		WorkerManager::idleWorkerList.push_front(unit);
	}
}

void WorkerManager::removeWorker(Unit unit) {
	WorkerManager::idleWorkerList.remove(unit);
	WorkerManager::repairWorkerList.remove(unit);
	WorkerManager::removeFromResourceMap(unit);
	WorkerManager::mineralWorkerList.remove_if([unit](std::array<BWAPI::Unit, 2> x) { return x.at(0) == unit; });
	WorkerManager::gasWorkerList.remove_if([unit](std::array<BWAPI::Unit, 2> x) { return x.at(0) == unit; });
}

void WorkerManager::removeFromResourceMap(Unit unit) {
	std::array<Unit, 2> workerToRemove = {nullptr, nullptr};
	for (auto& ar : WorkerManager::mineralWorkerList) {
		if (ar[0] == unit) {
			workerToRemove = ar;
			break;
		}
	}
	if (workerToRemove != *(new std::array<Unit, 2>{nullptr, nullptr})) {
		for (auto& ar : WorkerManager::gasWorkerList) {
			if (ar[0] == unit) {
				workerToRemove = ar;
				break;
			}
		}
	}

	if (workerToRemove != *(new std::array<Unit, 2>{nullptr, nullptr})) {

		int resourceGroup = workerToRemove[1]->getResourceGroup();

		for (auto& ar : resourceMap[resourceGroup]) {
			if (ar[0] == workerToRemove[1]) {
				for (int i = 1; i < 4; i++) {
					if (ar[i] == workerToRemove[0]) {
						for (int j = i; j < 3; j++) {
							ar[j] = ar[j + 1];
						}
						ar[3] = nullptr;
						break;
						break;
					}
				}
			}
		}
	}
}

void WorkerManager::addResource(Unit resource) {
	//Broodwar->sendText("Adding a Mineral patch");
	int resourceGroup = resource->getResourceGroup();
	
	resourceMap[resourceGroup].push_front(*new std::array<BWAPI::Unit, 4>{resource, nullptr, nullptr, nullptr});

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