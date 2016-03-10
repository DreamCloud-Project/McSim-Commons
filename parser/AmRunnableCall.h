#ifndef AM_RUNNABLE_CALL_H
#define AM_RUNNABLE_CALL_H

#include "AmRunnableClass.h"

namespace DCApplication {

class AmTask;
// forward declaration

/**
 * This class represents a runnable call node
 * in the task graph entries
 */
class AmRunnableCall {

private:
	AmRunnableClass *runClass;
	AmTask *task;
	pair<int, string> deadline;
	unsigned int idIntask;

public:

	// Constructor / Destructor
	AmRunnableCall(AmRunnableClass *runClass_, AmTask *task_,
			unsigned int idInTask_);
	~AmRunnableCall();

	// Getters
	AmRunnableClass * getRunClass();
	AmTask *getTask();
	unsigned int getIdInTask();

	// Deadline management
	void setDeadline(pair<int, string> DeadlineIn);
	int getDeadlineValue();
	string getDeadlineUnit();

	// Print
	void print();
};

}

#endif
