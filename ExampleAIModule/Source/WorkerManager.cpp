#include "WorkerManager.h"
#include <iostream>

using namespace BWAPI;

void WorkerManager::manageWorkers() {
	manageBuildWorkers();
	assignIdleWorkers();
	manageRepairWorkers();
	manageMineralWorkers();
	manageGasWorkers();

	//drawWorkerJobs();
}

void WorkerManager::manageBuildWorkers() {
	auto it = buildWorkerList.begin();

	while (it != buildWorkerList.end())
	{
		// if our worker is idle
		if ((*it)->isIdle()){
			idleWorkerList.push_front(*it);
			it = buildWorkerList.erase(it);
		}
		else {
			it++;
		}
	}
}

void WorkerManager::assignIdleWorkers() {
	//Broodwar->sendText("Managing idle workers");
	for (BWAPI::Unit w : idleWorkerList)
	{
		Unit nearestBase = w->getClosestUnit(Filter::IsResourceDepot);
		//Broodwar->sendText("Working with SCV");

		int id = ccToID[nearestBase];

		//Broodwar << "Mineral Cluster: " << id << "!" << std::endl;

		auto last = resourceMap[id].back();  //Get an iterator to the last value in the list

		int targetIndex = -1;  //Set the target index to invalid

		for (int i = 1; i < 4; i++) {
			if (last[i] == nullptr) {
				targetIndex = i;
				break;
			}
		}

		for (auto &ar : resourceMap[id])
		{
			if (ar[targetIndex] == nullptr && w->gather(ar[0])) {
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
	}

	idleWorkerList.clear();
}

void WorkerManager::manageRepairWorkers() {
	
}

void WorkerManager::manageMineralWorkers() {
	for (std::array<BWAPI::Unit, 2> a : WorkerManager::mineralWorkerList) {
		if (!a[0]->isGatheringMinerals()) {
			a[0]->gather(a[1]);
		}
		else if ((a[0]->getOrder() == Orders::MoveToMinerals) && (a[0]->getOrderTarget() != a[1])) {
			a[0]->gather(a[1]);
		}
	}

}

void WorkerManager::manageGasWorkers() {
	for (std::array<BWAPI::Unit, 2> a : WorkerManager::gasWorkerList) {
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

void WorkerManager::drawWorkerJobs() {
	for (Unit w : WorkerManager::idleWorkerList) {
		BWAPI::Position pos = w->getPosition();
		BWAPI::Order order = w->getOrder();
		Broodwar->registerEvent([pos, order](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, order.c_str()); },   // action
			nullptr,    // condition
			Broodwar->getLatencyFrames());  // frames to run
	}
	for (Unit w : WorkerManager::repairWorkerList) {
		BWAPI::Position pos = w->getPosition();
		BWAPI::Order order = w->getOrder();
		Broodwar->registerEvent([pos, order](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, order.c_str()); },   // action
			nullptr,    // condition
			Broodwar->getLatencyFrames());  // frames to run
	}
	for (Unit w : WorkerManager::buildWorkerList) {
		BWAPI::Position pos = w->getPosition();
		BWAPI::Order order = w->getOrder();
		Broodwar->registerEvent([pos, order](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, order.c_str()); },   // action
			nullptr,    // condition
			Broodwar->getLatencyFrames());  // frames to run
	}
	for (std::array<BWAPI::Unit, 2> a : WorkerManager::mineralWorkerList) {
		BWAPI::Position pos = a[0]->getPosition();
		BWAPI::Order order = a[0]->getOrder();
		Broodwar->registerEvent([pos, order](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, order.c_str()); },   // action
			nullptr,    // condition
			Broodwar->getLatencyFrames());  // frames to run
	}
	for (std::array<BWAPI::Unit, 2> a : WorkerManager::gasWorkerList) {
		BWAPI::Position pos = a[0]->getPosition();
		BWAPI::Order order = a[0]->getOrder();
		Broodwar->registerEvent([pos, order](Game*) { Broodwar->drawTextMap(pos, "%c%s", Text::White, order.c_str()); },   // action
			nullptr,    // condition
			Broodwar->getLatencyFrames());  // frames to run
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
	for (std::array<BWAPI::Unit, 2> ar : WorkerManager::mineralWorkerList) {
		if (ar[0] == unit) {
			workerToRemove = ar;
			break;
		}
	}
	if (workerToRemove != *(new std::array<Unit, 2>{nullptr, nullptr})) {
		for (std::array<BWAPI::Unit, 2> ar : WorkerManager::gasWorkerList) {
			if (ar[0] == unit) {
				workerToRemove = ar;
				break;
			}
		}
	}

	if (workerToRemove != *(new std::array<Unit, 2>{nullptr, nullptr})) {

		int resourceGroup = workerToRemove[1]->getResourceGroup();

		for (std::array<BWAPI::Unit, 4> ar : resourceMap[resourceGroup]) {
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
}

void WorkerManager::removeResource(Unit resource) {
	//Broodwar << "Resource mined out!" << std::endl;

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
	BWAPI::Unit builder = nullptr;

	if (!idleWorkerList.empty()) {
		builder = idleWorkerList.front();
		buildWorkerList.push_front(builder);
		idleWorkerList.pop_front();
	}

	else if (!mineralWorkerList.empty()){
		builder = mineralWorkerList.front()[0];
		removeFromResourceMap(mineralWorkerList.front()[0]);
		buildWorkerList.push_front(builder);
		mineralWorkerList.pop_front();
	}

	return builder;
}