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
	std::forward_list<BWAPI::TechType> researchOrder; //queue of order for researching
	bool isTest = true;
	bool troopFlip = true; //determines whether goliath or seige tank gets made
	
	

public:
	Producer();
	void trainTroops();
	void trainMarines();
	void trainSCVs();
	void trainFactoryTroops();
	void research();
	void addBuilding(BWAPI::Unit unit);
	void removeBuilding(BWAPI::Unit unit);
};