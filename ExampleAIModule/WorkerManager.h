#pragma once
#include <forward_list>
#include <BWAPI.h>

class WorkerManager
{
private:
	std::forward_list<BWAPI::Unit> workerList;

public:
	WorkerManager::WorkerManager();

	void addWorker(BWAPI::Unit unit);

};