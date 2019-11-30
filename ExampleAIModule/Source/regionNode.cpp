#include "regionNode.h"

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

bool regionNode::operator <(const regionNode &other) {
	return (nodePriority < other.nodePriority);
}

bool regionNode::operator >(const regionNode &other) {
	return (nodePriority > other.nodePriority);
}

bool regionNode::operator <=(const regionNode &other) {
	return (nodePriority <= other.nodePriority);
}

bool regionNode::operator >=(const regionNode &other) {
	return (nodePriority >= other.nodePriority);
}

void regionNode::operator =(const regionNode &other) {
	nodeRegion = other.nodeRegion;
	nodePriority = other.nodePriority;
	steps = other.steps;
}

int regionNode::getPriority() {
	return nodePriority;
}

int regionNode::getSteps() {
	return steps;
}

BWAPI::Region regionNode::getRegion() {
	return nodeRegion;
}