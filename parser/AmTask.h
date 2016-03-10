#ifndef AM_TASK_H
#define AM_TASK_H

#include <iostream>
#include <vector>
#include "dcActEvent.h"
#include "dcCore.h"
#include "dcTag.h"

namespace DCApplication {
using namespace std;

class AmRunnableCall;
// forward declaration

class AmTask {

	string Name;
	int Priority;
	bool PriorityValid;
	int MultipleTaskActivationLimit;
	bool MultipleTaskActivationLimitValid;
	string Preemption;
	pair<int, string> Deadline;
	vector<AmRunnableCall *> runnableCalls;
	dcActEvent *ActEvent_;
	int Size;
	vector<string> Resource;
	dcTag* tag;
	string ID;
	int TaskID;

public:
	AmTask(string NameIn);
	~AmTask();
	void SetName(string NameIn);
	string GetName();
	int GetPriority();
	void SetPriority(int PriorityIn);
	bool GetPriorityValid();
	int GetMultipleTaskActivationLimit();
	void SetMultipleTaskActivationLimit(int MultipleTaskActivationLimitIn);
	bool GetMultipleTaskActivationLimitValid();
	string GetPreemption();
	void SetPreemption(string PreemptionIn);
	void SetDeadline(pair<int, string> DeadlineIn);
	int GetDeadlineValue();
	string GetDeadlineUnit();
	int GetSize();
	void SetSize(int SizeIn);
	void AddRunnableCall(AmRunnableCall *runnCall);
	AmRunnableCall* GetRunnableCall(int Index);
	int GetNoOfRunnables();
	void Print();
	void SetActEvent(dcActEvent* ActEventIn);
	dcActEvent* GetActEvent();
	vector<string> GetResource();
	void SetResource(string ResourceIn);

	vector<AmRunnableCall*> GetRunnableCalls();
	void SetTag(dcTag* tagIn);
	dcTag* GetTag();

	string GetID();
	void SetID(string IDIn);

	int GetTaskID();
	void SetTaskID(int TaskIDIn);

};
}

#endif // AM_TASK_H
