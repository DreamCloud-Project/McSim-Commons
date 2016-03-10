#ifndef DC_RUNNABLE_CALL_H_
#define DC_RUNNABLE_CALL_H_

#include <iostream>
#include <string>
#include <vector>

#include "dcActEvent.h"
#include "dcInstruction.h"
#include "dcPeriodicEvent.h"
#include "dcRemoteAccessInstruction.h"
#include "dcRunnableEdge.h"
#include "dcRunnableExecutionTime.h"
#include "dcSporadicEvent.h"
#include "dcTag.h"

namespace DCApplication {
using namespace std;

// Forward declarations
class dcRunnableEdge;
class dcTask;

class dcRunnableCall {

public:
	dcRunnableCall(string NameIn, unsigned int idInTask_, dcTask *task_);

	vector<dcRunnableCall*> GetListOfEnables();
	vector<dcRunnableCall*> GetListOfEnablers();
	dcActEvent* GetActEvent();
	void SetActEvent(dcActEvent* dcActEventIn);
	string GetRunClassName();
	void SetRunnableClassId(string IDIn);
	string GetRunClassId();
	void SetEdges(dcRunnableEdge* RunEdgeIn);
	dcRunnableEdge* GetEdges();
	void SetNext(dcRunnableCall *NextIn);
	dcRunnableCall* GetNext();
	string GetRunSource();
	void SetRunSource(string RunSourceIn);
	vector<string> GetRunClassNameDestinations();
	void SetRunClassNameDestinations(vector<string> runClassNameDestinations);
	void SetTags(dcTag* TagsIn);
	dcTag* GetTags();
	int GetSize();
	void SetSize(int SizeIn);
	void SetInstructions(vector<dcInstruction*> InstructionIn);
	int GetPriority();
	void SetPriority(int PriorityIn);
	string GetPreemption();
	void SetPreemption(string PreemptionIn);
	vector<dcInstruction*> GetAllInstructions();
	vector<string> GetReleasedTasks();
	void SetReleasedTasks(vector<string> releasedTaskIn);
	void SetOrderedTaskReleased(bool OrderedTaskReleasedIn);
	bool GetOrderedTaskReleased();
	dcRunnableCall* GetTail();
	void SetTail(dcRunnableCall* TailIn);
	int GetRunClassCounter();
	void SetRunClassCounter(int RunnableIDIn);
	unsigned long GetPeriodInNano();
	unsigned long GetOffsetInNano();
	void SetPeriodicallyExecuted(int executed);
	void SetEnabledBy(int x);
	unsigned int GetEnabledBy();
	vector<string> GetReadLabelsIdsBySize();
	dcTask * GetTask();
	unsigned int GetIdInTask();
	unsigned int GetPeriodId();
	void IncPeriodId();

	// Deadline management
	unsigned long int GetDeadlineValue();
	unsigned long int GetDeadlineValueInNano();
	string GetDeadlineUnit();
	void SetDeadline(pair<int, string> deadline_);

	// Wave id management
	int GetWaveID();
	void SetWaveID(int waveID_);

private:
	dcActEvent *actEvent;

	// Identification of this runnable call
	string runClassId;
	string runClassName;
	string runCallId;
	unsigned int idInTask;
	dcTask *task;

	// Graph of runnable (for dependencies) management
	dcRunnableEdge *runEdge;
	vector<string> runClassNameDestinations;

	// Flat list of runnables management
	dcRunnableCall *nextRunnableCall;
	dcRunnableCall *tail;

	pair<unsigned long int, string> Deadline;
	dcTag* Tags;
	vector<dcInstruction*> Instructions;
	vector<dcInstruction*> readInstructions;
	int Size;
	vector<string> ReleasedTasks;
	int priority;
	int runnableClassCounter;
	int enabledBy;
	bool OrderedTaskReleased;
	int waveID;
	unsigned int periodId;
};

}
#endif /* RUNNABLE_H_ */
