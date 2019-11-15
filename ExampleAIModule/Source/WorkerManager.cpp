#include "WorkerManager.h"

using namespace BWAPI;

WorkerManager::WorkerManager() {
	WorkerManager::workerList = {};
}

void WorkerManager::manageWorkers() {
	for (auto &w : workerList)
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

void WorkerManager::addWorker(Unit unit) {
	WorkerManager::workerList.push_front(unit);
}

void WorkerManager::removeWorker(Unit unit) {
	WorkerManager::workerList.remove(unit);
}