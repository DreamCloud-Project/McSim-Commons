/*
 * dcRunnableInstance.cpp
 *
 *  Created on: Nov 24, 2015
 *      Author: manu
 */

#include "../../../Platform_Src/simulators-commons/parser/dcRunnableInstance.h"

#include "../../../Platform_Src/simulators-commons/parser/dcTask.h"

namespace DCApplication {

dcRunnableInstance::dcRunnableInstance(dcRunnableCall *runCall_) :
		runCall(runCall_) {
	periodId = runCall->GetPeriodId();
	runCall->IncPeriodId();
	releaseTime = 0;
	mappingTime = 0;
	coreReceiveTime = 0;
	startTime = 0;
	completionTime = 0;
}

dcRunnableInstance::~dcRunnableInstance() {
}

dcRunnableCall * dcRunnableInstance::getRunCall() {
	return runCall;
}

// Timing information
unsigned long int dcRunnableInstance::GetReleaseTime() {
	return releaseTime;
}
unsigned long int dcRunnableInstance::GetMappingTime() {
	return mappingTime;
}
unsigned long int dcRunnableInstance::GetCoreReceiveTime() {
	return coreReceiveTime;
}
unsigned long int dcRunnableInstance::GetStartTime() {
	return startTime;
}
unsigned long int dcRunnableInstance::GetCompletionTime() {
	return completionTime;
}
void dcRunnableInstance::SetReleaseTime(unsigned long int releaseTime_) {
	releaseTime = releaseTime_;
}
void dcRunnableInstance::SetMappingTime(unsigned long int mappingTime_) {
	mappingTime = mappingTime_;
}
void dcRunnableInstance::SetCoreReceiveTime(
		unsigned long int coreReceiveTime_) {
	coreReceiveTime = coreReceiveTime_;
}
void dcRunnableInstance::SetStartTime(unsigned long int startTime_) {
	startTime = startTime_;
}
void dcRunnableInstance::SetCompletionTime(unsigned long int completionTime_) {
	completionTime = completionTime_;
}

unsigned int dcRunnableInstance::GetPeriodId() {
	return periodId;
}

string dcRunnableInstance::GetUniqueID() {
	string taskName = getRunCall()->GetTask()->GetName();
	string runName = getRunCall()->GetRunClassName();
	return taskName + "_" + runName + "_"
			+ std::to_string(runCall->GetIdInTask()) + "_"
			+ std::to_string(periodId);
}
}
