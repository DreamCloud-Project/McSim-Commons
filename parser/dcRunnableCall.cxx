#include "dcRunnableCall.h"

#include <climits>

namespace DCApplication {
using namespace std;

dcRunnableCall::dcRunnableCall(string n, unsigned int idInTask_, dcTask *task_) :
		actEvent(NULL), runClassName(n), runCallId(""), idInTask(idInTask_), task(
				task_), runEdge(
		NULL), nextRunnableCall(NULL), tail(NULL), Tags(
		NULL), Size(0), priority(0), runnableClassCounter(-1), enabledBy(1), OrderedTaskReleased(
				true), waveID(0), periodId(0) {
}

dcActEvent* dcRunnableCall::GetActEvent() {
	return actEvent;
}

void dcRunnableCall::SetActEvent(dcActEvent* dcActEventIn) {
	actEvent = dcActEventIn;
}

dcTask * dcRunnableCall::GetTask() {
	return task;
}

unsigned int dcRunnableCall::GetIdInTask() {
	return idInTask;
}

unsigned long dcRunnableCall::GetPeriodInNano() {
	unsigned long period = 0;
	dcActEvent* event = GetActEvent();
	if (event != NULL) {
		string eventType = event->GetType();
		if (eventType == "stimuli:Periodic") {
			dcPeriodicEvent* periodic = static_cast<dcPeriodicEvent*>(event);
			pair<int, string> recurrence = periodic->GetRecurrence();
			if (recurrence.second == "ns") {
				period = recurrence.first;
			} else if (recurrence.second == "us") {
				period = recurrence.first * 1E3;
			} else if (recurrence.second == "ms") {
				period = recurrence.first * 1E6;
			} else {
				cerr << "Unit not supported in dcApplication::GetHyperPeriod";
				exit(-1);
			}
		} else if (eventType == "stimuli:Sporadic") {
			dcSporadicEvent* sporadic = static_cast<dcSporadicEvent*>(event);
			// TODO: Handle this properly
			period = sporadic->GetLowerBound();
		}
	}
	return period;
}

unsigned long dcRunnableCall::GetOffsetInNano() {
	unsigned long offset = 0;
	dcActEvent* event = GetActEvent();
	if (event != NULL) {
		string eventType = event->GetType();
		if (eventType == "stimuli:Periodic") {
			dcPeriodicEvent* periodic = static_cast<dcPeriodicEvent*>(event);
			pair<int, string> offsetPair = periodic->GetOffset();
			if (offsetPair.second == "ns") {
				offset = offsetPair.first;
			} else if (offsetPair.second == "us") {
				offset = offsetPair.first * 1E3;
			} else if (offsetPair.second == "ms") {
				offset = offsetPair.first * 1E6;
			} else {
				cerr << "Unit not supported in dcApplication::GetHyperPeriod";
				exit(-1);
			}
		} else if (eventType == "stimuli:Sporadic") {
			// TODO Handle this properly
			dcSporadicEvent* sporadic = static_cast<dcSporadicEvent*>(event);
			offset = sporadic->GetLowerBound();
		}
	}
	return offset;
}

string dcRunnableCall::GetRunClassName() {
	return runClassName;
}

void dcRunnableCall::SetEdges(dcRunnableEdge* EdgesIn) {
	runEdge = EdgesIn;
}

void dcRunnableCall::SetRunnableClassId(string IDIn) {
	runClassId = IDIn;
}

string dcRunnableCall::GetRunClassId() {
	return runClassId;
}

dcRunnableEdge* dcRunnableCall::GetEdges() {
	return runEdge;
}

void dcRunnableCall::SetNext(dcRunnableCall* NextIn) {
	nextRunnableCall = NextIn;
}

dcRunnableCall* dcRunnableCall::GetNext() {
	return nextRunnableCall;
}

string dcRunnableCall::GetRunSource() {
	return runCallId;
}

void dcRunnableCall::SetRunSource(string RunSourceIn) {
	runCallId = RunSourceIn;
}

vector<string> dcRunnableCall::GetRunClassNameDestinations() {
	return runClassNameDestinations;
}

void dcRunnableCall::SetRunClassNameDestinations(
		vector<string> RunDestinationIn) {
	runClassNameDestinations = RunDestinationIn;
}

void dcRunnableCall::SetTags(dcTag* TagsIn) {
	Tags = TagsIn;
}

dcTag* dcRunnableCall::GetTags() {
	return Tags;
}

void dcRunnableCall::SetInstructions(vector<dcInstruction*> InstructionsIn) {
	Instructions = InstructionsIn;
}

void dcRunnableCall::SetSize(int SizeIn) {
	Size = SizeIn;
}

int dcRunnableCall::GetSize() {
	return Size;
}

vector<dcInstruction*> dcRunnableCall::GetAllInstructions() {
	return Instructions;
}

vector<string> dcRunnableCall::GetReleasedTasks() {
	return ReleasedTasks;
}

void dcRunnableCall::SetReleasedTasks(vector<string> ReleasedTaskIn) {
	ReleasedTasks = ReleasedTaskIn;
}

void dcRunnableCall::SetOrderedTaskReleased(bool OrderedTaskReleasedIn) {
	OrderedTaskReleased = OrderedTaskReleasedIn;
}

bool dcRunnableCall::GetOrderedTaskReleased() {
	return OrderedTaskReleased;
}

unsigned long int dcRunnableCall::GetDeadlineValue() {
	return Deadline.first;
}

void dcRunnableCall::SetDeadline(pair<int, string> deadline_) {
	Deadline = deadline_;
}

string dcRunnableCall::GetDeadlineUnit() {
	return Deadline.second;
}

unsigned long int dcRunnableCall::GetDeadlineValueInNano() {
	if (GetDeadlineUnit() == "s") {
		return GetDeadlineValue() * 1E9;
	} else if (GetDeadlineUnit() == "ms") {
		return GetDeadlineValue() * 1E6;
	} else if (GetDeadlineUnit() == "us") {
		return GetDeadlineValue() * 1E3;
	} else if (GetDeadlineUnit() == "ns") {
		return GetDeadlineValue();
	} else {
		//cerr << "Unsupported deadline unit: " << GetDeadlineUnit() << endl;
		//exit(-1);
		return ULLONG_MAX;
	}
}

int dcRunnableCall::GetPriority() {
	return priority;
}

void dcRunnableCall::SetPriority(int priority_) {
	priority = priority_;
}

void dcRunnableCall::SetTail(dcRunnableCall* TailIn) {
	tail = TailIn;
}

dcRunnableCall* dcRunnableCall::GetTail() {
	return tail;
}

void dcRunnableCall::SetRunClassCounter(int IDIn) {
	runnableClassCounter = IDIn;
}

int dcRunnableCall::GetRunClassCounter() {
	return runnableClassCounter;
}

vector<dcRunnableCall*> dcRunnableCall::GetListOfEnables() {
	vector<dcRunnableCall*> ListRun;
	dcRunnableEdge* MyRunEdges = GetEdges();
	if (MyRunEdges != NULL) {
		while (MyRunEdges != NULL) {
			if (MyRunEdges->GetType() == 1) {
				ListRun.push_back(MyRunEdges->GetConnectTo());
			}
			MyRunEdges = MyRunEdges->GetNext();
		}
	}

	return ListRun;
}

vector<dcRunnableCall*> dcRunnableCall::GetListOfEnablers() {
	vector<dcRunnableCall*> ListRun;
	dcRunnableEdge* MyRunEdges = GetEdges();
	if (MyRunEdges == NULL) {

	} else {
		while (MyRunEdges != NULL) {
			if (MyRunEdges->GetType() == 0) {
				ListRun.push_back(MyRunEdges->GetConnectTo());
			}
			MyRunEdges = MyRunEdges->GetNext();
		}
	}

	return ListRun;
}

void dcRunnableCall::SetEnabledBy(int x) {
	enabledBy = x;
}

unsigned int dcRunnableCall::GetEnabledBy() {
	return enabledBy;
}

vector<string> dcRunnableCall::GetReadLabelsIdsBySize() {
	vector<pair<int, string>> temp_;
	int instructionSize = GetAllInstructions().size();
	for (int i = 0; i < instructionSize; i++) {
		DCApplication::dcInstruction* inst = GetAllInstructions().at(i);
		string instrName = inst->GetName();
		if (instrName == "sw:LabelAccess") {
			dcRemoteAccessInstruction *rinst =
					static_cast<dcRemoteAccessInstruction*>(inst);
			if (!rinst->GetWrite()) {
				dcLabel *l = rinst->GetLabel();
				temp_.push_back(std::make_pair(l->GetSize(), l->GetID()));
			}
		}
	}
	sort(temp_.begin(), temp_.end());
	vector<string> insts;
	for (vector<pair<int, string>>::size_type j = 0; j < temp_.size(); j++) {
		insts.push_back(temp_.at(j).second);
	}
	return insts;
}

// Wave id management
int dcRunnableCall::GetWaveID() {
	return waveID;
}

void dcRunnableCall::SetWaveID(int waveID_) {
	waveID = waveID_;
}

unsigned int dcRunnableCall::GetPeriodId() {
	return periodId;
}

void dcRunnableCall::IncPeriodId() {
	periodId++;
}
}
