#pragma once
#include <BWAPI.h>

class regionNode {
	BWAPI::Region nodeRegion;
	int nodePriority;
	int steps;

public:
	regionNode::regionNode(BWAPI::Region region, int priority, int stepsNum);
	regionNode::regionNode(const regionNode &otherNode);
	virtual bool operator <(const regionNode &other);
	virtual bool operator >(const regionNode &other);
	virtual bool operator <=(const regionNode &other);
	virtual bool operator >=(const regionNode &other);
	virtual void operator =(const regionNode &other);
	virtual int regionNode::getPriority();
	virtual int regionNode::getSteps();
	virtual  BWAPI::Region regionNode::getRegion();
};