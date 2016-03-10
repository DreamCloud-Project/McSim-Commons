/*
 * dcRunnableInstance.h
 *
 *  Created on: Nov 24, 2015
 *      Author: manu
 */

#ifndef SRC_PLATFORM_SRC_SRC_CXX_MODULES_DREAMCLOUD_PLATFORM_SCLIB_PARSER_DCRUNNABLEINSTANCE_H_
#define SRC_PLATFORM_SRC_SRC_CXX_MODULES_DREAMCLOUD_PLATFORM_SCLIB_PARSER_DCRUNNABLEINSTANCE_H_

#include "dcRunnableCall.h"

namespace DCApplication {

class dcRunnableInstance {

private:
	dcRunnableCall * runCall;
	unsigned long int releaseTime;
	unsigned long int mappingTime;
	unsigned long int coreReceiveTime;
	unsigned long int startTime;
	unsigned long int completionTime;
	unsigned int periodId;

public:
	dcRunnableInstance(dcRunnableCall *runCall_);
	virtual ~dcRunnableInstance();
	dcRunnableCall * getRunCall();

	// Timing information
	unsigned long int GetReleaseTime();
	unsigned long int GetMappingTime();
	unsigned long int GetCoreReceiveTime();
	unsigned long int GetStartTime();
	unsigned long int GetCompletionTime();
	void SetReleaseTime(unsigned long int releaseTime_);
	void SetMappingTime(unsigned long int mappingTime_);
	void SetCoreReceiveTime(unsigned long int coreReceiveTime_);
	void SetStartTime(unsigned long int startTime_);
	void SetCompletionTime(unsigned long int completionTime_);

	unsigned int GetPeriodId();
	string GetUniqueID();
};
}

#endif /* SRC_PLATFORM_SRC_SRC_CXX_MODULES_DREAMCLOUD_PLATFORM_SCLIB_PARSER_DCRUNNABLEINSTANCE_H_ */
