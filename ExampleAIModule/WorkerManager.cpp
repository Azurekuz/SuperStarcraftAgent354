#include "WorkerManager.h"

using namespace BWAPI;

WorkerManager::WorkerManager() {
	WorkerManager::workerList = {};
}

void WorkerManager::addWorker(Unit unit) {
	WorkerManager::workerList.push_front(unit);
}