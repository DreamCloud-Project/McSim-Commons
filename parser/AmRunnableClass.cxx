#include "../../../Platform_Src/simulators-commons/parser/AmRunnableClass.h"

#include "../../../Platform_Src/simulators-commons/parser/dcExecutionCyclesConstantInstruction.h"
#include "../../../Platform_Src/simulators-commons/parser/dcExecutionCyclesConstantInstruction.h"
#include "../../../Platform_Src/simulators-commons/parser/dcExecutionCyclesDeviationInstruction.h"

namespace DCApplication {
AmRunnableClass::AmRunnableClass(string NameIn) :
		Name(NameIn), Tags(NULL), ID(""), Size(0) {
}

AmRunnableClass::~AmRunnableClass() {
	vector<dcInstruction*>::iterator itInstructions;
	for (itInstructions = Instructions.begin();
			itInstructions != Instructions.end(); ++itInstructions) {
		delete *itInstructions;
	}
}

void AmRunnableClass::SetName(string NameIn) {
	Name = NameIn;
}

string AmRunnableClass::GetName() {
	return Name;
}

void AmRunnableClass::SetTags(dcTag* TagsIn) {
	Tags = TagsIn;
}

dcTag* AmRunnableClass::GetTags() {
	return Tags;
}

void AmRunnableClass::AddInstruction(dcInstruction *InstructionIn) {
	Instructions.push_back(InstructionIn);
}

void AmRunnableClass::SetSize(int SizeIn) {
	Size = SizeIn;
}

int AmRunnableClass::GetSize() {
	return Size;
}

void AmRunnableClass::SetID(string IDIn) {
	ID = IDIn;
}

string AmRunnableClass::GetID() {
	return ID;
}

void AmRunnableClass::Print() {
	vector<dcInstruction*>::iterator it;
	int i;
	cout << "Name: " << Name << endl;
	for (i = 0, it = Instructions.begin(); it != Instructions.end();
			++it, i++) {
		cout << "	Instruction no. " << i << ": ";
		(*it)->Print();
		cout << endl;
	}

}

bool AmRunnableClass::CheckIfIncludesExecutionCyclesDeviationInstruction() {
	bool result = false;
	vector<dcInstruction*>::iterator it;
	for (it = Instructions.begin(); it != Instructions.end(); ++it) {
		if (dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it) != NULL) {
			result = true;
			break;
		}
	}
	return result;
}

double AmRunnableClass::GetWCETInTick() {
	vector<dcInstruction*>::iterator it;
	double nWCET = 0.0;
	for (it = Instructions.begin(); it != Instructions.end(); ++it) {
		if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it)) != NULL) {
			nWCET +=
					(dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetUpperBound();
		} else if ((dynamic_cast<dcExecutionCyclesConstantInstruction*>(*it))
				!= NULL) {
			nWCET +=
					(dynamic_cast<dcExecutionCyclesConstantInstruction*>(*it))->GetValue();
		}
	}
	return nWCET;
}

string AmRunnableClass::GetTheFirstDeviationType() {
	vector<dcInstruction*>::iterator it;
	for (it = Instructions.begin(); it != Instructions.end(); ++it) {
		if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it)) != NULL) {
			return (dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetType();
		}
	}
	return "";
}

double AmRunnableClass::GetTheFirstDeviationRemainPromille() {
	vector<dcInstruction*>::iterator it;
	for (it = Instructions.begin(); it != Instructions.end(); ++it) {
		if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it)) != NULL) {
			if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetRemainPromilleValid()) {
				return (dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetRemainPromille();
			} else {
				return 0.0;
			}
		}
	}
	return 0.0;
}

double AmRunnableClass::GetTheFirstDeviationLowerBound() {
	vector<dcInstruction*>::iterator it;
	for (it = Instructions.begin(); it != Instructions.end(); ++it) {
		if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it)) != NULL) {
			if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetLowerBoundValid()) {
				return (dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetLowerBound();
			} else {
				return 0;
			}
		}
	}
	return 0;
}

double AmRunnableClass::GetTheFirstDeviationUpperBound() {
	vector<dcInstruction*>::iterator it;
	for (it = Instructions.begin(); it != Instructions.end(); ++it) {
		if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it)) != NULL) {
			if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetUpperBoundValid()) {
				return (dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetUpperBound();
			} else {
				return 0;
			}
		}
	}
	return 0;
}

double AmRunnableClass::GetTheFirstDeviationMean() {
	vector<dcInstruction*>::iterator it;
	for (it = Instructions.begin(); it != Instructions.end(); ++it) {
		if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it)) != NULL) {
			if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetMeanValid()) {
				return (dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetMean();
			} else {
				return 0;
			}
		}
	}
	return 0;
}

double AmRunnableClass::GetTheFirstDeviationStandardDeviation() {
	vector<dcInstruction*>::iterator it;
	for (it = Instructions.begin(); it != Instructions.end(); ++it) {
		if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it)) != NULL) {
			if ((dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetSDValid()) {
				return (dynamic_cast<dcExecutionCyclesDeviationInstruction*>(*it))->GetSD();
			} else {
				return 0.0;
			}
		}
	}
	return 0.0;
}

vector<dcInstruction*> AmRunnableClass::GetAllInstructions() {
	return Instructions;
}

vector<string> AmRunnableClass::GetReleasedTasks() {
	return ReleasedTasks;
}

void AmRunnableClass::AddReleasedTask(string ReleasedTaskIn) {
	ReleasedTasks.push_back(ReleasedTaskIn);
}

string AmRunnableClass::GetRunSource() {
	return RunSource;
}

void AmRunnableClass::SetRunSource(string RunSourceIn) {
	RunSource = RunSourceIn;
}

vector<string> AmRunnableClass::GetRunDestination() {
	return RunDestination;
}

void AmRunnableClass::AddRunDestination(string RunDestinationIn) {
	RunDestination.push_back(RunDestinationIn);
}

vector<string> AmRunnableClass::GetRunnableDestination() {
	return RunDestination;
}

void AmRunnableClass::SetOrderedTaskReleased(bool OrderedTaskReleasedIn) {
	OrderedTaskReleased = OrderedTaskReleasedIn;
}

bool AmRunnableClass::GetOrderedTaskReleased() {
	return OrderedTaskReleased;
}

void AmRunnableClass::SetRunnableExecutionTime(
		dcRunnableExecutionTime* MyExecutionTimeIn) {
	ExecutionTime = MyExecutionTimeIn;
}

dcRunnableExecutionTime* AmRunnableClass::GetRunnableExecutionTime() {
	return ExecutionTime;
}

void AmRunnableClass::SetCounter(int IDIn) {
	counter = IDIn;
}

int AmRunnableClass::GetCounter() {
	return counter;
}

}
