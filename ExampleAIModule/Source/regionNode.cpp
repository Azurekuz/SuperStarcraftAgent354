#include "regionNode.h"
regionNode::regionNode() {

}

regionNode::regionNode(BWAPI::Region region, int priority, int stepsNum) {
	nodeRegion = region;
	nodePriority = priority;
	steps = stepsNum;
}

regionNode::regionNode(const regionNode &otherNode) {
	nodeRegion = otherNode.nodeRegion;
	nodePriority = otherNode.nodePriority;
	steps = otherNode.steps;
}

bool regionNode::operator<(const regionNode & b)
{
	return (nodePriority < b.nodePriority);
}



bool regionNode::operator >(regionNode const &b) {
	return (nodePriority > b.nodePriority);
}

bool regionNode::operator <=(regionNode const &b) {
	return (nodePriority <= b.nodePriority);
}

bool regionNode::operator >=(regionNode const &b) {
	return (nodePriority >= b.nodePriority);
}

void regionNode::operator =(regionNode const &other) {
	nodeRegion = other.nodeRegion;
	nodePriority = other.nodePriority;
	steps = other.steps;
}

bool regionNode::operator()(regionNode const &a, regionNode const &b) {
	return a.nodePriority > b.nodePriority;
}

int regionNode::setPriority(int priority) {
	return nodePriority;
}

int regionNode::setSteps(int steps) {
	return steps;
}

BWAPI::Region regionNode::setRegion(BWAPI::Region region) {
	return nodeRegion;
}

int regionNode::getPriority() {
	return nodePriority;
}

int regionNode::getSteps() {
	return steps;
}

const BWAPI::Region regionNode::getRegion() {
	return nodeRegion;
}