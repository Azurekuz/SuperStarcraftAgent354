#include "Task.h"

Task::Task() {

}

std::string Task::getTask() {
	return taskName;
}

int Task::getPriority() {
	return priority;
}

bool Task::operator <(const Task &b) {
	return priority < b.priority;
}
bool Task::operator >(Task const &b) {
	return priority > b.priority;
}
bool Task::operator <=(Task const &b) {
	return priority <= b.priority;
}
bool Task::operator >=(Task const &b) {
	return priority >= b.priority;
}
void Task::operator =(Task const &other) {
	priority = other.priority;
	taskName = other.taskName;
}