#pragma once
#include <BWAPI.h>

class regionNode {
	BWAPI::Region nodeRegion;
	int steps;

public:
	int nodePriority;
	regionNode::regionNode();
	regionNode::regionNode(BWAPI::Region region, int priority, int stepsNum);
	regionNode::regionNode(const regionNode &otherNode);
	virtual bool regionNode::operator <(const regionNode &b);
	virtual bool regionNode::operator >(regionNode const &b);
	virtual bool regionNode::operator <=(regionNode const &b);
	virtual bool regionNode::operator >=(regionNode const &b);
	virtual void regionNode::operator =(regionNode const &other);
	bool regionNode::operator()(regionNode const &a, regionNode const &b);
	virtual int regionNode::setPriority(int priority);
	virtual int regionNode::setSteps(int steps);
	virtual BWAPI::Region regionNode::setRegion(BWAPI::Region region);
	virtual int regionNode::getPriority();
	virtual int regionNode::getSteps();
	virtual const BWAPI::Region regionNode::getRegion();
};