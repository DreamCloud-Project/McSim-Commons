#include "../../../Platform_Src/simulators-commons/parser/AmRunnableCall.h"

namespace DCApplication {

AmRunnableCall::AmRunnableCall(AmRunnableClass *runClass_, AmTask *task_,
		unsigned int idInTask_) :
		runClass(runClass_), task(task_), idIntask(idInTask_) {
}

void AmRunnableCall::setDeadline(pair<int, string> DeadlineIn) {
	deadline = DeadlineIn;
}

int AmRunnableCall::getDeadlineValue() {
	return deadline.first;
}

string AmRunnableCall::getDeadlineUnit() {
	return deadline.second;
}

void AmRunnableCall::print() {
	runClass->Print();
}

AmRunnableClass* AmRunnableCall::getRunClass() {
	return runClass;
}

AmTask* AmRunnableCall::getTask() {
	return task;
}

unsigned int AmRunnableCall::getIdInTask() {
	return idIntask;
}

}
