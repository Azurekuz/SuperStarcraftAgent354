#pragma once
#include <BWAPI.h>

class Task {
	std::string taskName;

	public:
		int priority;

		Task::Task();
		virtual int Task::getPriority();
		virtual std::string Task::getTask();
		virtual bool Task::operator <(const Task &b);
		virtual bool Task::operator >(Task const &b);
		virtual bool Task::operator <=(Task const &b);
		virtual bool Task::operator >=(Task const &b);
		virtual void Task::operator =(Task const &other);

};