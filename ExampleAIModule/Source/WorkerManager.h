#pragma once
#include <forward_list>
#include <list>
#include <array>
#include <map>
#include <iterator>
#include <BWAPI.h>

class WorkerManager
{
private:
	std::list<BWAPI::Unit> idleWorkerList{};
	std::list<BWAPI::Unit> repairWorkerList{};
	std::list<BWAPI::Unit> buildWorkerList{};
	std::forward_list<std::array<BWAPI::Unit, 2>> mineralWorkerList{};
	std::forward_list<std::array<BWAPI::Unit, 2>> gasWorkerList{};

	std::map<BWAPI::Unit, int> ccToID;  //This map is designed to get the ID of the resource nodes associated with it
	std::map<int, std::list<std::array<BWAPI::Unit, 4>>> resourceMap;

public:
	void manageWorkers();
	void assignIdleWorkers();
	void manageRepairWorkers();
	void manageBuildWorkers();
	void manageMineralWorkers();
	void manageGasWorkers();
	void drawWorkerJobs();
	void drawMineralStatus();


	void addWorker(BWAPI::Unit unit);
	void removeWorker(BWAPI::Unit unit);
	void removeFromResourceMap(BWAPI::Unit unit);
	void addResource(BWAPI::Unit resource);
	void removeResource(BWAPI::Unit resource);
	void addCC(BWAPI::Unit cc);
	void removeCC(BWAPI::Unit cc);
	BWAPI::Unit getBuilder(BWAPI::TilePosition loc);

};