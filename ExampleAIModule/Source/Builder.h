#pragma once

#include <BWAPI.h>
class Builder {
	//Desmond, I was thinking about the best way to do things, and I think our best bet is to get the location we want to build at first, and then pass that as an argument
	//to getBuilder, and I can then get the closest idle worker to build it.  -Orion
	void Builder::buildBarracks(BWAPI::UnitType Terran);
	void Builder::buildSupply(BWAPI::Unit supplyBuilder, BWAPI::UnitType supplyBuilderType);
};