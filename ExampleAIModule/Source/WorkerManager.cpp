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
	auto it = idleWorkerList.begin();

	while (it != idleWorkerList.end())
	{
		Unit nearestBase = (*it)->getClosestUnit(Filter::IsResourceDepot);
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

		if (targetIndex != -1) {

			bool found = false;

			for (auto &ar : resourceMap[id])
			{
				if (ar[targetIndex] == nullptr && (*it)->gather(ar[0])) {
					//Broodwar << "Targetindex: " << targetIndex << "!" << std::endl;
					//Broodwar << "SCV assigned to patch: " << ar[0] << "!" << std::endl;
					ar[targetIndex] = (*it);
					//Broodwar << "SCV address: " << ar[targetIndex] << "!" << std::endl;
					if (ar[0]->getType() == BWAPI::UnitTypes::Resource_Mineral_Field) {
						WorkerManager::mineralWorkerList.push_front({ (*it), ar[0] });
					}
					else {
						WorkerManager::gasWorkerList.push_front({ (*it), ar[0] });
					}
					it = idleWorkerList.erase(it);

					found = true;

					break;
				}
			}
			if (!found) {
				it++;
			}
		}
		else {
			it++;
		}
	}
}

void WorkerManager::manageRepairWorkers() {
	auto it = repairWorkerList.begin();

	while (it != repairWorkerList.end())
	{
		// if our worker is idle
		if ((*it)->isIdle()) {
			idleWorkerList.push_front(*it);
			it = buildWorkerList.erase(it);
		}
		else {
			it++;
		}
	}
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
	WorkerManager::mineralWorkerList.remove_if([unit](std::array<BWAPI::Unit, 2> x) { return x[0] == unit; });
	WorkerManager::gasWorkerList.remove_if([unit](std::array<BWAPI::Unit, 2> x) { return x[0] == unit; });
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
	std::list<std::array<Unit, 4>> cluster = resourceMap[resource->getResourceGroup()];
	auto it = cluster.begin();

	while (it != cluster.end())
	{
		if ((*it)[0] == resource) {
			for (int i = 1; i < 4; i++) {
				Unit worker = (*it)[i];
				idleWorkerList.push_front(worker);
				if ((*it)[0]->getType() == UnitTypes::Resource_Mineral_Field)
					WorkerManager::mineralWorkerList.remove_if([worker](std::array<BWAPI::Unit, 2> x) { return x[0] == worker; });
				else
					WorkerManager::gasWorkerList.remove_if([worker](std::array<BWAPI::Unit, 2> x) { return x[0] == worker; });
			}
			it = cluster.erase(it);
		}
		else {
			it++;
		}
	}
	resourceMap[resource->getResourceGroup()] = cluster;
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

void WorkerManager::removeCC(Unit cc) {
	auto it = ccToID.find(cc);
	ccToID.erase(it);
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

	/*if (builder != nullptr) {
		Broodwar << "Got worker!" << std::endl;
	}
	else {
		Broodwar << "Failed to get worker!" << std::endl;
	}*/

	return builder;
}