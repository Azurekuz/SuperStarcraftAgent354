#pragma once
#include <BWAPI.h>
#include <forward_list>

class Producer
{
private:
	std::forward_list<BWAPI::Unit> commandcentersList;
	std::forward_list<BWAPI::Unit> barracksList;
	std::list<BWAPI::Unit> factoriesList;
	std::forward_list<BWAPI::Unit> starportsLists;
	std::forward_list<BWAPI::Unit> machineshopsList;
	std::forward_list<BWAPI::Unit> armoriesList;
	std::forward_list<BWAPI::Unit> otherList;
	bool isTest = true;
	bool troopFlip = true; //determines whether goliath or seige tank gets made
	
	

public:
	/* Going to need many more functions, this is just to start*/
	Producer();
	void trainTroops();
	void trainMarines();
	void trainSCVs();
	void trainFactoryTroops();
	void research();
	void addBuilding(BWAPI::Unit unit);
	void removeBuilding(BWAPI::Unit unit);
};