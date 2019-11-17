#pragma once
#include <BWAPI.h>
#include <forward_list>

class Producer
{
private:
	std::forward_list<BWAPI::Unit> commandcentersList;
	std::forward_list<BWAPI::Unit> barracksList;
	std::forward_list<BWAPI::Unit> factoriesList;
	std::forward_list<BWAPI::Unit> starportsLists;
	std::forward_list<BWAPI::Unit> machineshopsList;
	std::forward_list<BWAPI::Unit> armoriesList;
	std::forward_list<BWAPI::Unit> otherList;
	

public:
	/* Going to need many more functions, this is just to start*/
	Producer();
	void produceTroops(BWAPI::Unit u);
	void research();
	void addBuilding(BWAPI::Unit unit);
	void removeBuilding(BWAPI::Unit unit);
	void buildSupply(BWAPI::Unit supplyBuilder, BWAPI::UnitType supplyProviderType); //remove once Builder checks supply
};