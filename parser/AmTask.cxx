#include "AmTask.h"
#include "AmRunnableCall.h"
#include "AmRunnableCall.h"

namespace DCApplication
{
	AmTask::AmTask(string NameIn): Name(NameIn), Priority(0), PriorityValid(false), ActEvent_(NULL), Size(0), tag(NULL) /*activation(NULL), next(NULL)*/ {
	}

	AmTask::~AmTask() {}


	void AmTask::SetName(string NameIn) {
		Name=NameIn;
	}

	string AmTask::GetName() {
		return Name;
	}

	int AmTask::GetPriority() {
		return Priority;
	}

	void AmTask::SetPriority(int PriorityIn) {
		Priority=PriorityIn;
	}

	bool AmTask::GetPriorityValid() {
		return PriorityValid;
	}

	int AmTask::GetMultipleTaskActivationLimit() {
		return MultipleTaskActivationLimit;
	}

	void AmTask::SetMultipleTaskActivationLimit(int MultipleTaskActivationLimitIn) {
		MultipleTaskActivationLimit=MultipleTaskActivationLimitIn;
	}

	bool AmTask::GetMultipleTaskActivationLimitValid() {
		return MultipleTaskActivationLimitValid;
	}

	void AmTask::SetPreemption(string PreemptionIn) {
		Preemption=PreemptionIn;
	}

	string AmTask::GetPreemption() {
		return Preemption;
	}

	void AmTask::SetDeadline(pair<int,string> DeadlineIn) {
		Deadline=DeadlineIn;
	}

	int AmTask::GetDeadlineValue() {
		return Deadline.first;
	}

	string AmTask::GetDeadlineUnit(){
		return Deadline.second;
	}

	void AmTask::SetSize(int SizeIn) {
		Size=SizeIn;
	}

	int AmTask::GetSize() {
		return Size;
	}

	void AmTask::AddRunnableCall(AmRunnableCall *runnCall) {
		runnableCalls.push_back(runnCall);
	}

	void AmTask::Print() {
		cout << "Name: " << Name;
		cout << " period - ";
		ActEvent_->Print();
		cout << endl;
		vector<AmRunnableCall*>::iterator it;
		int i;
		for(i=0,it=runnableCalls.begin();it!=runnableCalls.end();++it,i++) {
			cout << "Runnable no:" << i << endl;
			(*it)->print();
		}
	}


	void AmTask::SetActEvent(dcActEvent* StimulusIn) {
		ActEvent_=StimulusIn;
	}

	dcActEvent*  AmTask::GetActEvent() {
		return ActEvent_;
	}

	void AmTask::SetResource(string ResourceIn){
		Resource.push_back(ResourceIn);
	}
	vector<string> AmTask::GetResource(){
		return Resource;
	}

	AmRunnableCall*  AmTask::GetRunnableCall(int Index) {
		return runnableCalls[Index];
	}

	int AmTask::GetNoOfRunnables() {
		return runnableCalls.size();
	}

	// get runnables
	vector<AmRunnableCall*> AmTask::GetRunnableCalls(){
		return runnableCalls;
	}

	void AmTask::SetTag(dcTag* tagIn){
		tag= tagIn;
	}

	dcTag* AmTask::GetTag(){
		return tag;
	}

	string AmTask::GetID(){
		return ID;
	}
	void AmTask::SetID(string IDIn){
		ID = IDIn;
	}
	
	int AmTask::GetTaskID(){
		return TaskID;
	}
	void AmTask::SetTaskID(int TaskIDIn){
		TaskID = TaskIDIn;
	}

}
