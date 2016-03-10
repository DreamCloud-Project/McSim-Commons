#include "dcApplication.h"

#include <queue>
#include <iostream>
#include <ratio>
#include <algorithm> 
#include <numeric>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <random>
#include <time.h> 
#include <stdlib.h>
#include <string>
#include <sstream>
#include <set>

#include "AmRunnableCall.h"
#include "../utils/Math.hxx"

using namespace std;

namespace DCApplication {

dcApplication::dcApplication() :
		FirstTask(NULL), Visited(false), Size(0), EdgeSize(0), Head(NULL) {
}

dcApplication::~dcApplication() {
}

dcTaskGraph* dcApplication::createGraph(string NameIn) {
	dcTaskGraph* dcTaskGraphIn = new dcTaskGraph();
//		cout << "Task Graph " << NameIn << " created" << endl;
	return dcTaskGraphIn;
}

void dcApplication::AddTasks(dcTaskGraph* dcTaskGraphIn,
		AmApplication* AmApplicationIn) {
	vector<AmTask*> dcTaskIn = AmApplicationIn->GetAllTask();
	for (vector<AmTask*>::iterator it = dcTaskIn.begin(); it != dcTaskIn.end();
			++it) {
		dcTask* TaskIn = new dcTask((*it)->GetName());
		TaskIn->SetTag((*it)->GetTag());
		TaskIn->SetPriority((*it)->GetPriority());
		TaskIn->SetMultipleTaskActivationLimit(
				(*it)->GetMultipleTaskActivationLimit());
		TaskIn->SetPreemption((*it)->GetPreemption());
		TaskIn->SetDeadline(
				pair<int, string>((*it)->GetDeadlineValue(),
						(*it)->GetDeadlineUnit()));
		TaskIn->SetActEvent((*it)->GetActEvent());
		TaskIn->SetSize((*it)->GetSize());
		TaskIn->SetResources((*it)->GetResource());
		TaskIn->SetID((*it)->GetID());
		TaskIn->SetTaskID((*it)->GetTaskID());
		if (dcTaskGraphIn->GetHead() == NULL && dcTaskGraphIn->GetTail() == NULL) {
			dcTaskGraphIn->SetHead(TaskIn);
			dcTaskGraphIn->SetTail(TaskIn);
		} else {
			dcTaskGraphIn->GetTail()->SetNext(TaskIn);
			dcTaskGraphIn->SetTail(TaskIn);
		}
	}

}

void dcApplication::PrintTasks(dcTaskGraph* dcTaskGraphIn) {
	dcTask *MyTask = dcTaskGraphIn->GetHead();
	if (MyTask == NULL) {
		cout << "No Task in the dcApplication " << endl;
	} else {
		while (MyTask != NULL) {
			cout << "Task is : " + MyTask->GetName() << endl;
			MyTask = MyTask->GetNext();
		}

	}
}

vector<dcTask*> dcApplication::GetAllTasks(dcTaskGraph* dcTaskGraphIn) {
	vector<dcTask*> MyTasks;

	dcTask *MyTask = dcTaskGraphIn->GetHead();
	if (MyTask == NULL) {
		cout << "No Task in the dcApplication " << endl;
	} else {
		while (MyTask != NULL) {
			MyTasks.push_back(MyTask);
			MyTask = MyTask->GetNext();
			;
		}
	}

	return MyTasks;
}

void dcApplication::AdddcTaskEdges(dcTaskGraph* dcTaskGraphIn,
		AmApplication* AmApplicationIn) {
	vector<dcActEvent*> dcActionEventIn = AmApplicationIn->GetAllActEvent();
	if (dcTaskGraphIn->GetHead() == 0) {
		cerr << "No tasks in the dcApplication, cannot add edge(s)" << endl;
		exit(-1);
	} else {
		for (vector<dcActEvent*>::iterator it = dcActionEventIn.begin();
				it != dcActionEventIn.end(); ++it) {
			string ActEventType = (*it)->GetType();
			if (ActEventType == "stimuli:InterProcess") {
				vector<AmTask*> SrcTasks = (*it)->GetAllSource();
				vector<AmTask*> DestTasks = (*it)->GetAllDestination();

				dcTask* SrcTask = NULL;
				dcTask* DestTask = NULL;
				SrcTask = dcTaskGraphIn->GetHead();
				DestTask = dcTaskGraphIn->GetHead();

				if (SrcTasks.size() > DestTasks.size()) {
					size_t i, j = 0;

					for (i = 0; i < SrcTasks.size(); i++) {

						//scan for the source
						while (SrcTask != 0) {
							if (SrcTask->GetTaskID()
									== SrcTasks.at(i)->GetTaskID()) {
								//source found
								break;
							} else {
								SrcTask = SrcTask->GetNext();
							}
						}

						for (j = 0; j < DestTasks.size(); j++) {
							//scan for the dest
							while (DestTask != 0) {
								if (DestTask->GetTaskID()
										== DestTasks.at(j)->GetTaskID()) {
									//source found
									break;
								} else {
									DestTask = DestTask->GetNext();
								}
							}
							//Create task edge
							dcTaskEdge* MydcTaskEdge = new dcTaskEdge(); //front edge
							MydcTaskEdge->SetType(1);

							dcTaskEdge* MydcTaskEdge2 = new dcTaskEdge(); //back edge
							MydcTaskEdge2->SetType(0);

							MydcTaskEdge->SetConnectTo(DestTask);
							MydcTaskEdge2->SetConnectTo(SrcTask);

							if (SrcTask->GetEdges() == 0) {
								SrcTask->SetEdges(MydcTaskEdge);
								SrcTask->GetEdges()->SetTail(MydcTaskEdge);
							} else {
								SrcTask->GetEdges()->GetTail()->SetNext(
										MydcTaskEdge);
								SrcTask->GetEdges()->SetTail(MydcTaskEdge);
							}

							if (DestTask->GetEdges() == 0) {
								DestTask->SetEdges(MydcTaskEdge2);
								DestTask->GetEdges()->SetTail(MydcTaskEdge2);
							} else {
								DestTask->GetEdges()->GetTail()->SetNext(
										MydcTaskEdge2);
								DestTask->GetEdges()->SetTail(MydcTaskEdge2);

							}
						}
					}

				} else {
					size_t i, j = 0;
					for (i = 0; i < SrcTasks.size(); i++) {

						//scan for the source
						while (SrcTask != 0) {
							if (SrcTask->GetTaskID()
									== SrcTasks.at(i)->GetTaskID()) {
								//source found
								break;
							} else {
								SrcTask = SrcTask->GetNext();
							}
						}

						for (j = 0; j < DestTasks.size(); j++) {

							//scan for the dest
							while (DestTask != 0) {
								if (DestTask->GetTaskID()
										== DestTasks.at(j)->GetTaskID()) {
									//source found
									break;
								} else {
									DestTask = DestTask->GetNext();
								}
							}
							//Create task edge
							dcTaskEdge* MydcTaskEdge = new dcTaskEdge();
							MydcTaskEdge->SetType(1);

							dcTaskEdge* MydcTaskEdge2 = new dcTaskEdge();
							MydcTaskEdge2->SetType(0);

							MydcTaskEdge->SetConnectTo(DestTask);
							MydcTaskEdge2->SetConnectTo(SrcTask);

							if (SrcTask->GetEdges() == 0) {
								SrcTask->SetEdges(MydcTaskEdge);
								SrcTask->GetEdges()->SetTail(MydcTaskEdge);
							} else {
								SrcTask->GetEdges()->GetTail()->SetNext(
										MydcTaskEdge);
								SrcTask->GetEdges()->SetTail(MydcTaskEdge);
							}

							if (DestTask->GetEdges() == 0) {
								DestTask->SetEdges(MydcTaskEdge2);
								DestTask->GetEdges()->SetTail(MydcTaskEdge2);
							} else {
								DestTask->GetEdges()->GetTail()->SetNext(
										MydcTaskEdge2);
								DestTask->GetEdges()->SetTail(MydcTaskEdge2);
							}
						}
					}
				}
			}
		}
	}
}

vector<dcTask*> dcApplication::GetdcTaskEdges(dcTaskGraph* dcTaskGraphIn,
		dcTask* TaskIn) {
	vector<dcTask*> MyTasks;

	dcTask* MyTask = dcTaskGraphIn->GetHead();
	if (MyTask == 0) {
		cout << "No tasks in the dcApplication :(" << endl;
	} else {
		//get to the Task
		while (MyTask != 0) {
			if (MyTask->GetTaskID() == TaskIn->GetTaskID()) {
				break;
			} else {
				MyTask = MyTask->GetNext();
			}
		}

		if (MyTask == NULL) {
			cout << "Task :" << MyTask->GetName()
					<< " not found in dcApplication " << endl;
		} else {
			dcTaskEdge* MydcTaskEdge = MyTask->GetEdges();
			while (MydcTaskEdge != NULL) {
				MyTasks.push_back(MydcTaskEdge->GetConnectTo());
				MydcTaskEdge = MydcTaskEdge->GetNext();
			}
		}
	}
	return MyTasks;
}

/**
 * Adds the runnables from the given AM application
 * to the given DC task graph
 */
void dcApplication::AddRunnables(dcTaskGraph* dcTaskGraph,
		AmApplication* AmApplicationIn) {
	vector<AmTask*> amTasks = AmApplicationIn->GetAllTask();
	dcTask* curDctask = dcTaskGraph->GetHead();
	if (curDctask == 0) {
		cout << "No Tasks in the dcApplication, cannot add runnables" << endl;
	} else {

		// For each AM task
		for (vector<AmTask*>::iterator it = amTasks.begin();
				it != amTasks.end(); ++it) {
			AmTask* curAmTask = (*it);

			// Search for DC task corresponding to AM task
			while (curDctask != 0) {
				if (curDctask->GetTaskID() == curAmTask->GetTaskID()) {
					break;
				} else {
					curDctask = curDctask->GetNext();
				}
			}
			if (curDctask == 0) {
				cout << "Task: " << curDctask->GetName()
						<< " not found in the dcApplication" << endl;
			}

			// Create a DC runnable from the AM runnable
			else {
				vector<AmRunnableCall*> amRunnableCalls =
						curAmTask->GetRunnableCalls();
				for (vector<AmRunnableCall*>::iterator itRun =
						amRunnableCalls.begin(); itRun != amRunnableCalls.end();
						++itRun) {
					dcRunnableCall* dcRunCall = new dcRunnableCall(
							(*itRun)->getRunClass()->GetName(),
							(*itRun)->getIdInTask(), curDctask);
					vector<string> runDest =
							(*itRun)->getRunClass()->GetRunDestination();
					dcRunCall->SetRunSource(
							(*itRun)->getRunClass()->GetRunSource());
					dcRunCall->SetRunnableClassId(
							(*itRun)->getRunClass()->GetID());
					dcRunCall->SetTags((*itRun)->getRunClass()->GetTags());
					dcRunCall->SetSize((*itRun)->getRunClass()->GetSize());
					dcRunCall->SetInstructions(
							(*itRun)->getRunClass()->GetAllInstructions());
					dcRunCall->SetReleasedTasks(
							(*itRun)->getRunClass()->GetReleasedTasks());
					dcRunCall->SetOrderedTaskReleased(
							(*itRun)->getRunClass()->GetOrderedTaskReleased());
					dcRunCall->SetRunClassCounter(
							(*itRun)->getRunClass()->GetCounter());
					dcRunCall->SetActEvent(curDctask->GetActEvent());
					dcRunCall->SetDeadline(
							make_pair((*itRun)->getDeadlineValue(),
									(*itRun)->getDeadlineUnit()));
					dcRunCall->SetPriority((*itRun)->getTask()->GetPriority());

					if (runDest.size() > 0) {
						dcRunCall->SetRunClassNameDestinations(runDest);
					}
					if (curDctask->GetRunnableCalls() == 0) {
						curDctask->SetRunnables(dcRunCall);
						curDctask->GetRunnableCalls()->SetTail(dcRunCall);
					} else {
						curDctask->GetRunnableCalls()->GetTail()->SetNext(
								dcRunCall);
						curDctask->GetRunnableCalls()->SetTail(dcRunCall);
					}
				}
			}
		}
	}
}

/**
 * Returns a flat list (vector) of all the runnable
 * calls in the given task.
 */
vector<dcRunnableCall*> dcApplication::GetTaskRunnableCalls(
		dcTaskGraph* dcTaskGraphIn, dcTask* TaskIn) {

	vector<dcRunnableCall*> runnableCalls;
	dcTask* MyTask = dcTaskGraphIn->GetHead();
	if (MyTask == 0) {
		cout << "No tasks in the dcApplication :(" << endl;
	} else {
		//get to the Task
		while (MyTask != 0) {
			if (MyTask->GetTaskID() == TaskIn->GetTaskID()) {
				break;
			} else {
				MyTask = MyTask->GetNext();
			}
		}

		if (MyTask == NULL) {
			cout << "Task :" << MyTask->GetName()
					<< " not found in dcApplication " << endl;
		} else {
			dcRunnableCall* runCall = MyTask->GetRunnableCalls();
			while (runCall != NULL) {
				runnableCalls.push_back(runCall);
				runCall = runCall->GetNext();
			}
		}
	}
	return runnableCalls;
}

vector<dcRunnableCall*> dcApplication::GetRunnableCall(dcTask* task,
		string runClassId) {

	vector<dcRunnableCall*> result;
	dcRunnableCall* runCall = task->GetRunnableCalls();
	while (runCall != NULL) {
		if (runCall->GetRunClassId() == runClassId) {
			result.push_back(runCall);
		} else {
			runCall = runCall->GetNext();
		}
	}
	return result;
}

vector<int> dcApplication::GetAllPriorities(dcTaskGraph* dcTaskGraphIn) {
	dcTask* task = dcTaskGraphIn->GetHead();
	vector<int> res;
	while (task != 0) {
		int prio = task->GetPriority();
		if (std::find(res.begin(), res.end(), prio) == res.end()) {
			res.push_back(prio);
			sort(res.begin(), res.end());
		}
		task = task->GetNext();
	}
	return res;
}

vector<dcRunnableCall*> dcApplication::GetAllRunnables(
		dcTaskGraph* dcTaskGraphIn) {
	dcTask* MyTask = dcTaskGraphIn->GetHead();
	vector<dcRunnableCall*> MyRunnableReturn;

	if (MyTask == 0) {
		cout << "No tasks in the dcApplication :(" << endl;
	} else {
		while (MyTask != 0) {
			dcRunnableCall* MyRunnables = MyTask->GetRunnableCalls();
			while (MyRunnables != NULL) {
				MyRunnableReturn.push_back(MyRunnables);
				MyRunnables = MyRunnables->GetNext();
			}
			MyTask = MyTask->GetNext();
		}
	}
	return MyRunnableReturn;
}

vector<dcInstruction*> dcApplication::GetAllInstructions(
		dcTaskGraph* dcTaskGraphIn) {
	dcTask* task = dcTaskGraphIn->GetHead();
	vector<dcInstruction*> insts;
	while (task != 0) {
		dcRunnableCall* runnable = task->GetRunnableCalls();
		while (runnable != NULL) {
			vector<dcInstruction*> toAdd = runnable->GetAllInstructions();
			insts.insert(insts.end(), toAdd.begin(), toAdd.end());
			runnable = runnable->GetNext();
		}
		task = task->GetNext();
	}
	return insts;
}

void dcApplication::AdddcRunnableEdges(dcTaskGraph* dcTaskGraphIn,
		bool seqDep) {

	dcTask* task = dcTaskGraphIn->GetHead();
	if (task == 0) {
		cout << "No Tasks in the dcApplication, cannot add runnables" << endl;
	} else {
		while (task != NULL) {

			dcRunnableCall* run = task->GetRunnableCalls();
			dcRunnableCall* prevRun = NULL;
			while (run != NULL) {

				// Add callee runnables dependency
				size_t DestRunSize = run->GetRunClassNameDestinations().size();
				for (size_t i = 0; i < DestRunSize; i++) {

					// Create runnable edges
					dcRunnableEdge* frontEdge = new dcRunnableEdge();
					frontEdge->SetType(1);
					dcRunnableEdge* backEdge = new dcRunnableEdge();
					backEdge->SetType(0);

					// Assert destination runnable call is in the same task
					// than source runnable call and assert there is only
					// a single runnable call with this run class name
					// in the current task
					string destRunClassName =
							run->GetRunClassNameDestinations().at(i);
					vector<dcRunnableCall*> destRunCalls = GetRunnableCall(task,
							destRunClassName);
					if (destRunCalls.empty()) {
						cerr
								<< "Dependency between runnable of different tasks or to unknown runnable"
								<< endl;
						exit(-1);
					} else if (destRunCalls.size() > 1) {
						cerr
								<< "Dependency to runnable having several instances. Don't know which one to use."
								<< endl;
						exit(-1);
					}

					dcRunnableCall *destRun = destRunCalls.at(0);
					frontEdge->SetConnectTo(destRun);
					backEdge->SetConnectTo(run);

					// Add edges to run
					if (run->GetEdges() == NULL) {
						run->SetEdges(frontEdge);
						run->GetEdges()->SetTail(frontEdge);
					} else {
						run->GetEdges()->GetTail()->SetNext(frontEdge);
						run->GetEdges()->SetTail(frontEdge);
					}
					if (destRun->GetEdges() == NULL) {
						destRun->SetEdges(backEdge);
						destRun->GetEdges()->SetTail(backEdge);
					} else {
						destRun->GetEdges()->GetTail()->SetNext(backEdge);
						destRun->GetEdges()->SetTail(backEdge);
					}
				}

				// Add sequential dependency if needed
				if (seqDep && prevRun != NULL) {

					//Create runnable edges
					dcRunnableEdge* frontEdge = new dcRunnableEdge();
					frontEdge->SetType(1);
					dcRunnableEdge* backEdge = new dcRunnableEdge();
					backEdge->SetType(0);
					frontEdge->SetConnectTo(run);
					backEdge->SetConnectTo(prevRun);

					// Add edges to run
					if (prevRun->GetEdges() == NULL) {
						prevRun->SetEdges(frontEdge);
						prevRun->GetEdges()->SetTail(frontEdge);
					} else {
						prevRun->GetEdges()->GetTail()->SetNext(frontEdge);
						prevRun->GetEdges()->SetTail(frontEdge);
					}
					if (run->GetEdges() == NULL) {
						run->SetEdges(backEdge);
						run->GetEdges()->SetTail(backEdge);
					} else {
						run->GetEdges()->GetTail()->SetNext(backEdge);
						run->GetEdges()->SetTail(backEdge);
					}

				}
				prevRun = run;
				run = run->GetNext();
			}
			task = task->GetNext();
		}
	}
}

/*
 void dcApplication::AdddcRunnableEdges(dcTaskGraph* dcTaskGraphIn){

 dcTask* MyTask = dcTaskGraphIn->GetHead();
 if(MyTask == 0){
 cout<<"No Tasks in the dcApplication, cannot add runnables" << endl;
 }else{
 while(MyTask != NULL){
 //check whether task has runnables
 if(MyTask->GetRunnables() != NULL){

 dcRunnable* MyRunnables = MyTask->GetRunnables();
 while(MyRunnables != NULL){
 size_t DestRunSize = MyRunnables->GetRunnableDestination().size();
 if(DestRunSize > 0){
 for(size_t i = 0; i < DestRunSize; i++){
 dcRunnableEdge* MydcRunnableEdge = new dcRunnableEdge();
 MydcRunnableEdge->SetConnectTo(GetRunnable(dcTaskGraphIn, MyRunnables->GetRunnableDestination().at(i)));
 if(MyRunnables->GetEdges() == NULL){
 MyRunnables->SetEdges(MydcRunnableEdge);
 MyRunnables->GetEdges()->SetTail(MydcRunnableEdge);
 }else{
 MyRunnables->GetEdges()->GetTail()->SetNext(MydcRunnableEdge);
 MyRunnables->GetEdges()->SetTail(MydcRunnableEdge);
 }
 }
 }

 MyRunnables = MyRunnables->GetNext();
 }

 }


 MyTask = MyTask->GetNext();
 }
 }
 }
 */

vector<dcRunnableCall*> dcApplication::GetdcRunnableEdges(
		dcTaskGraph* dcTaskGraphIn, dcTask* TaskIn,
		dcRunnableCall* RunnableIn) {
	dcTask* MyTask = dcTaskGraphIn->GetHead();
	vector<dcRunnableCall*> MyRunnableReturn;

	if (MyTask == 0) {
		cout << "No tasks in the dcApplication :(" << endl;
	} else {
		//get to the task
		while (MyTask != NULL) {
			if (MyTask->GetTaskID() == TaskIn->GetTaskID()) {
				break;
			} else {
				MyTask = MyTask->GetNext();
			}
		}

		if (MyTask == NULL) {
			cout << "Task: " << MyTask->GetName()
					<< " is not in the dcApplication " << endl;
		} else {
			dcRunnableCall* MyRunnables = MyTask->GetRunnableCalls();
			while (MyRunnables != NULL) {
				if (MyRunnables->GetRunClassId()
						== RunnableIn->GetRunClassId()) {
					dcRunnableEdge* MyRunEdges = MyRunnables->GetEdges();
					if (MyRunEdges == NULL) {
						cout << RunnableIn->GetRunClassName()
								<< " has no destination" << endl;
					} else {
						while (MyRunEdges != NULL) {
							if (MyRunEdges->GetType() == 1) {
								MyRunnableReturn.push_back(
										MyRunEdges->GetConnectTo());
								MyRunEdges = MyRunEdges->GetNext();
							}

						}
					}
				}

				MyRunnables = MyRunnables->GetNext();
			}
		}
	}
	return MyRunnableReturn;
}

/*
 vector<dcRunnable*> dcApplication::GetdcRunnableEdges(dcTaskGraph* dcTaskGraphIn, dcTask* TaskIn, dcRunnable* RunnableIn){
 dcTask* MyTask = dcTaskGraphIn->GetHead();
 vector<dcRunnable*> MyRunnableReturn;

 if(MyTask == 0){
 cout << "No tasks in the dcApplication :(" << endl;
 }else{
 //get to the task
 while(MyTask != NULL){
 if(MyTask->GetTaskID() == TaskIn->GetTaskID()){
 break;
 }else{
 MyTask = MyTask->GetNext();
 }
 }

 if(MyTask == NULL){
 cout << "Task: " << MyTask->GetName() << " is not in the dcApplication " << endl;
 }else{
 dcRunnable* MyRunnables = MyTask->GetRunnables();
 while(MyRunnables != NULL){
 if(MyRunnables->GetID() == RunnableIn->GetID()){
 dcRunnableEdge* MyRunEdges = MyRunnables->GetEdges();
 if(MyRunEdges == NULL){
 cout << RunnableIn->GetName() << " has no destination" << endl;
 }else{
 while(MyRunEdges != NULL){
 MyRunnableReturn.push_back(MyRunEdges->GetConnectTo());
 MyRunEdges = MyRunEdges->GetNext();
 }
 }
 }

 MyRunnables = MyRunnables->GetNext();
 }
 }
 }
 return MyRunnableReturn;
 }
 */

//labels
vector<dcLabel*> dcApplication::GetAllLabels(AmApplication* AmApplicationIn) {
	return AmApplicationIn->GetAllLabels();
}

//cores
vector<dcCore*> dcApplication::GetCores(AmApplication* AmApplicationIn) {
	return AmApplicationIn->GetCores();
}

void dcApplication::PrintGraph(dcTaskGraph* dcTaskGraphIn, ofstream& myfile) {
	dcTask *MyTask = dcTaskGraphIn->GetHead();
	if (MyTask == NULL) {
		cout << "Task :" << MyTask->GetName() << " not found in dcGraph "
				<< endl;
	} else {
		if (myfile.is_open()) {
			while (MyTask != NULL) {
				myfile << "=====================================" << endl;
				myfile << MyTask->GetName() << " -> ";

				dcTaskEdge* Edge = MyTask->GetEdges();
				while (Edge != NULL) {
					if (Edge->GetType() == 1) { //forward edge
						myfile << Edge->GetConnectTo()->GetName() << " ";
					}

					Edge = Edge->GetNext();
				}

				myfile << endl;
				myfile << endl;

				//runnable dependency
				dcRunnableCall* MyRunnable = MyTask->GetRunnableCalls();
				while (MyRunnable != NULL) {

					myfile << MyRunnable->GetRunClassName() << " -> ";
					dcRunnableEdge* MyRunEdges = MyRunnable->GetEdges();

					while (MyRunEdges != NULL) {
						myfile << MyRunEdges->GetConnectTo()->GetRunClassName()
								<< " ";
						MyRunEdges = MyRunEdges->GetNext();
					}

					myfile << endl;
					MyRunnable = MyRunnable->GetNext();
				}
				MyTask = MyTask->GetNext();
				myfile << endl;
			}
		} else {
			myfile << "cannot open file" << endl;
		}

	}
	myfile << "======================================================" << endl;
}

void dcApplication::CreateGraphEntities(dcTaskGraph* dcTaskGraphIn,
		AmApplication* AmApplication, bool seqDep) {
	AddTasks(dcTaskGraphIn, AmApplication);
	AdddcTaskEdges(dcTaskGraphIn, AmApplication);
	AddRunnables(dcTaskGraphIn, AmApplication);
	AdddcRunnableEdges(dcTaskGraphIn, seqDep);
}

vector<dcTaskGraph*> dcApplication::GenerateMicroworkloads(
		dcTaskGraph *TaskGraphIn, AmApplication* dcApplicationIn, size_t width,
		size_t depth, size_t num) {
	clock_t start, end;
	start = clock();

	//basic patterns
	vector<dcTask*> Independent;
	vector<vector<dcTask*>> Fork;
	vector<vector<dcTask*>> Join;
	vector<vector<dcTask*>> Feedback;
	vector<vector<dcTask*>> Sequential;
	vector<dcTask*> SequenceTemp;
	vector<vector<dcTask*>> Further;
	vector<vector<dcTask*>> Independents;
	unsigned int NumOfTasks = GetAllTasks(TaskGraphIn).size(); //microworkload should be less than the size of the input application

	dcTask *MyTask = TaskGraphIn->GetHead();
	if (MyTask != NULL) {
		size_t count = 0;
		size_t count2 = 0; //counter for join tasks
		while (MyTask != NULL) {
			count = 0;
			count2 = 0;
			dcTaskEdge* Edge = MyTask->GetEdges();
			if (Edge == NULL) { //independent tasks
				dcTask* Temp = Clone(MyTask);
				Independent.push_back(Temp);
			}
			while (Edge != NULL) { //other structures

				if (Edge->GetType() == 1) { //front edge
					count = count + 1;
				}
				if (Edge->GetType() == 0) {
					count2++;
				}
				Edge = Edge->GetNext();
			}

			if (count2 > 1) { //join structure
				dcTask* Task = Clone(MyTask); //dest
				dcTaskEdge* EdgeT = MyTask->GetEdges(); //get back edges
				Task->SetEdges(0);
				vector<dcTask*> Tasks;
				Tasks.push_back(Task);
				while (EdgeT != NULL) {
					if (EdgeT->GetType() == 0) {
						dcTaskEdge *E = new dcTaskEdge();
						E->SetType(1);
						E->SetNext(0);
						dcTask* Temp = Clone(EdgeT->GetConnectTo()); //src task
						E->SetConnectTo(Task);

						if (Temp->GetEdges() == 0) {
							Temp->SetEdges(E);
							Temp->GetEdges()->SetTail(E);
						} else {
							Temp->GetEdges()->GetTail()->SetNext(E);
							Temp->GetEdges()->SetTail(E);
						}
						Tasks.push_back(Temp);
					}

					EdgeT = EdgeT->GetNext();
				}
				bool Exist = Existing(Tasks.size(), Join);
				if (Exist != 1) {
					Join.push_back(Tasks);
				}
			}

			if (count > 1) { //fork structure
				dcTask* Task = Clone(MyTask);
				dcTaskEdge* EdgeT = MyTask->GetEdges();
				Task->SetEdges(0);
				vector<dcTask*> Tasks;

				Tasks.push_back(Task);
				while (EdgeT != NULL) {

					if (EdgeT->GetType() == 1) {
						dcTaskEdge *E = new dcTaskEdge();
						E->SetType(1);
						E->SetNext(0);
						dcTask* Temp = Clone(EdgeT->GetConnectTo());
						E->SetConnectTo(Temp);

						if (Task->GetEdges() == 0) {
							Task->SetEdges(E);
							Task->GetEdges()->SetTail(E);
						} else {
							Task->GetEdges()->GetTail()->SetNext(E);
							Task->GetEdges()->SetTail(E);
						}
						Tasks.push_back(Temp);

					}

					EdgeT = EdgeT->GetNext();
				}
				bool Exist = Existing(Tasks.size(), Fork);
				if (Exist != 1) {
					Fork.push_back(Tasks);
				}

			}

			if (count == 1) { //sequence or feedback task
				FirstTask = MyTask;
				SequenceTemp = GetSequence(MyTask);
				if (SequenceTemp.size() > 1) {
					Result2.clear();
					for (size_t i = 0; i < SequenceTemp.size(); i++) {
						SequenceTemp.at(i)->SetVisited(0);
					}
					bool type = 0;

					dcTaskEdge* Edge = SequenceTemp.back()->GetEdges();
					while (Edge != NULL) {
						if (Edge->GetType() == 1) {
							if (Edge->GetConnectTo() == MyTask) {
								type = 1;
								break;
							}
						}
						Edge = Edge->GetNext();
					}
					if (type == 1) {
						bool Exist = Existing(SequenceTemp.size(), Feedback);
						if (Exist != 1) {
							Feedback.push_back(SequenceTemp);
						}

					} else {
						bool Exist = Existing(SequenceTemp.size(), Sequential);
						if (Exist != 1) {
							Sequential.push_back(SequenceTemp);
						}
					}
				}

			}

			//further workloads
			vector<pair<dcTask*, pair<string, int>>> Temp;
			string FirstType = "";
			int ForkDegree = 0;
			bool JoinFlag = false;
			Result = AnalyseTask(MyTask);
			vector<dcTask*> ForkTemp;
			vector<dcTask*> SeqTemp;

			if (Result.size() > 1 && !Result.empty()) {
				for (size_t i = 0; i < Result.size(); i++) {
					dcTask* Task = Result.at(i).first;
					string Type = Result.at(i).second.first;
					int Degree = Result.at(i).second.second;

					pair<dcTask*, pair<string, int>> Temp2;
					vector<dcTask*> Temp3;

					Temp2.first = Task;
					Temp2.second.first = Type;
					Temp2.second.second = Degree;

					if (ForkDegree != 0) {
						ForkDegree--;
					}
					if (Type == "J" && ForkDegree != 0) {
						ForkDegree = 0;
					}

					if (Type != "I") {
						if (i == 0) {
							Temp.push_back(Temp2);

							FirstType = Result.at(i).second.first;

							if (Type == "F" && ForkDegree == 0) {
								ForkDegree = Result.at(i).second.second;
							}
						} else {
							if (FirstType == Type) { //keep the chain
								Temp.push_back(Temp2);
							} else { // I have a further workload
								Temp.push_back(Temp2);

								if (ForkDegree == 0) {
									FirstType = Result.at(i).second.first;
									vector<dcTask*> Temp3;
									for (size_t j = 0; j < Temp.size(); j++) {
										if (j == 0) {
											dcTask* Original = Temp.at(j).first;
											dcTask* Cloned = Clone(
													Temp.at(j).first);

											Temp3.push_back(Cloned);
											dcTaskEdge* Edge =
													Original->GetEdges();

											while (Edge != NULL) {
												if (Edge->GetType() == 1) {
													dcTask* Temp_Task =
															Edge->GetConnectTo();
													if (Temp_Task != NULL) {
														dcTaskEdge* E =
																new dcTaskEdge();
														E->SetType(1);

														dcTask* Dest = Clone(
																Temp_Task);
														E->SetConnectTo(Dest);

														if (Cloned->GetEdges()
																== 0) {
															Cloned->SetEdges(E);
															Cloned->GetEdges()->SetTail(
																	E);
														} else {
															Cloned->GetEdges()->GetTail()->SetNext(
																	E);
															Cloned->GetEdges()->SetTail(
																	E);
														}
														Temp3.push_back(Dest);

													}

												}

												Edge = Edge->GetNext();
											}

										} else {
											dcTask* Original = Temp.at(j).first;
											dcTask* Cloned = Clone(
													Temp.at(j).first);
											string WhatType =
													Temp.at(j).second.first;

											//check whether Task is in the buffer
											bool Exist = IsExisting(Temp3,
													Original);

											if (Exist == 1) {
												//Searh for the task
												for (size_t k = 0;
														k < Temp3.size(); k++) {
													if (Original->GetID()
															== Temp3.at(k)->GetID()) {
														dcTaskEdge* Edge =
																Original->GetEdges();
														while (Edge != NULL) {
															if (Edge->GetType()
																	== 1) {
																dcTask* Dest =
																		Clone(
																				Edge->GetConnectTo());
																if (Dest != NULL) {
																	dcTaskEdge *E =
																			new dcTaskEdge();
																	E->SetType(
																			1);
																	E->SetConnectTo(
																			Dest);

																	if (Temp3.at(
																			k)->GetEdges()
																			== 0) {
																		Temp3.at(
																				k)->SetEdges(
																				E);
																		Temp3.at(
																				k)->GetEdges()->SetTail(
																				E);
																	} else {
																		Temp3.at(
																				k)->GetEdges()->GetTail()->SetNext(
																				E);
																		Temp3.at(
																				k)->GetEdges()->SetTail(
																				E);
																	}

																	Temp3.push_back(
																			Dest);
																}
															}
															Edge =
																	Edge->GetNext();
														}
													}
												}
											} else {
												Temp3.push_back(Cloned);

												if (WhatType == "J") {
													dcTaskEdge* Edge =
															Original->GetEdges();
													//back edges
													while (Edge != NULL) {
														if (Edge->GetType()
																== 0) { //connect back edges
															dcTaskEdge *E =
																	new dcTaskEdge();
															E->SetType(1);

															//get the source
															dcTask* Source =
																	Clone(
																			Edge->GetConnectTo());
															//Check if it's existing already
															bool Exist =
																	IsExisting(
																			Temp3,
																			Source);
															if (Exist == 1) {
																//get to the real source
																for (size_t k =
																		0;
																		k
																				< Temp3.size();
																		k++) {
																	if (Source->GetID()
																			== Temp3.at(
																					k)->GetID()) { //source found
																		E->SetConnectTo(
																				Cloned);
																		if (Temp3.at(
																				k)->GetEdges()
																				== 0) {
																			Temp3.at(
																					k)->SetEdges(
																					E);
																		} else {
																			E->SetNext(
																					Temp3.at(
																							k)->GetEdges());
																			Temp3.at(
																					k)->SetEdges(
																					E);
																		}
																		JoinFlag =
																				true;
																	}
																}
															}

														}
														Edge = Edge->GetNext();
													}

													if (JoinFlag == true) {
														Further.push_back(
																Temp3);
														//add Independent workload to further workloads if it exit
														for (size_t i = 0;
																i < 2; i++) {
															if (Independent.size()
																	> 0) {
																size_t index =
																		rand()
																				% (Independent.size()
																						- 1
																						+ 1)
																				+ 0;
																Further.back().push_back(
																		Independent.at(
																				index));
															}
														}
														JoinFlag = false;
													}
													dcTaskEdge* Edge2 =
															Original->GetEdges();
													while (Edge2 != NULL) {
														if (Edge2->GetType()
																== 1) {
															dcTask* Dest =
																	Clone(
																			Edge2->GetConnectTo());
															if (Dest != NULL) {
																dcTaskEdge *E =
																		new dcTaskEdge();
																E->SetType(1);
																E->SetConnectTo(
																		Dest);

																if (Cloned->GetEdges()
																		== 0) {
																	Cloned->SetEdges(
																			E);
																	Cloned->GetEdges()->SetTail(
																			E);
																} else {
																	Cloned->GetEdges()->GetTail()->SetNext(
																			E);
																	Cloned->GetEdges()->SetTail(
																			E);
																}
																Temp3.push_back(
																		Dest);
															}

														}
														Edge2 =
																Edge2->GetNext();
													}
												}
											}
										}
									}
									if (Temp3.size() < NumOfTasks) { //workload should be less than input model
										Further.push_back(Temp3);
										//add Independent workload to further workloads if it exit
										for (size_t i = 0; i < 3; i++) {
											if (Independent.size() > 0) {
												size_t index = rand()
														% (Independent.size()
																- 1 + 1) + 0;
												Further.back().push_back(
														Independent.at(index));
											}
										}
									}

								}
								if (Type == "F" && ForkDegree == 0) {
									ForkDegree = Result.at(i).second.second;
								}
							}
						}
					}

				}
			}

			Result.clear();
			MyTask = MyTask->GetNext();

		}
	}

	vector<vector<dcTask*>> CustomWorkloads;

	for (size_t i = 0; i < num; i++) {
		//user oriented workloads
		srand((unsigned int) time(NULL));
		vector<dcTask*> Tasks = GetAllTasks(TaskGraphIn);
		vector<dcTask*> Custom;

		dcTask *FirstTask = NULL;
		dcTask* MyForkTask = NULL;
		bool flag = false;
		vector<dcTask*> Seq;

		size_t count = 0;
		while (count < depth) {

			if (count == 0) { //sequential tasks
				size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
				FirstTask = Clone(Tasks.at(index));
				Custom.push_back(FirstTask);

			} else if (count == 1) { // fork
				size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
				MyForkTask = Clone(Tasks.at(index));
				dcTaskEdge *E = new dcTaskEdge();
				E->SetType(1);
				E->SetConnectTo(MyForkTask);

				if (FirstTask->GetEdges() == 0) {
					FirstTask->SetEdges(E);
					FirstTask->GetEdges()->SetTail(E);
				} else {
					FirstTask->GetEdges()->GetTail()->SetNext(E);
					FirstTask->GetEdges()->SetTail(E);
				}

				Custom.push_back(MyForkTask);

				//create the degree depending on the input width
				for (size_t i = 0; i < width; i++) {
					size_t index2 = rand() % (Tasks.size() - 1 + 1) + 0;
					dcTask* MyForkChildren = Clone(Tasks.at(index2));
					Custom.push_back(MyForkChildren);

					dcTaskEdge *E2 = new dcTaskEdge();
					E2->SetType(1);
					E2->SetConnectTo(MyForkChildren);

					if (MyForkTask->GetEdges() == 0) {
						MyForkTask->SetEdges(E2);
						MyForkTask->GetEdges()->SetTail(E2);
					} else {
						MyForkTask->GetEdges()->GetTail()->SetNext(E2);
						MyForkTask->GetEdges()->SetTail(E2);
					}
				}
			} else if (count == (depth - 1)) { //for to join
				if (Seq.size() == 0) {
					size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
					dcTask* MyTask = Clone(Tasks.at(index));
					Custom.push_back(MyTask);

					for (size_t i = 0; i < width; i++) {

						dcTaskEdge *E2 = new dcTaskEdge();
						E2->SetType(1);
						E2->SetConnectTo(MyTask);

						if (MyForkTask->GetEdges() == 0) {
							MyForkTask->SetEdges(E2);
							MyForkTask->GetEdges()->SetTail(E2);
						} else {
							MyForkTask->GetEdges()->GetTail()->SetNext(E2);
							MyForkTask->GetEdges()->SetTail(E2);
						}
					}

				} else { // sequence to join
					size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
					dcTask* MyTask = Clone(Tasks.at(index));
					Custom.push_back(MyTask);
					for (size_t i = 0; i < Seq.size(); i++) {

						dcTaskEdge *E = new dcTaskEdge();
						E->SetType(1);
						E->SetConnectTo(MyTask);

						if (Seq.at(i)->GetEdges() == 0) {
							Seq.at(i)->SetEdges(E);
							Seq.at(i)->GetEdges()->SetTail(E);
						} else {
							Seq.at(i)->GetEdges()->GetTail()->SetNext(E);
							Seq.at(i)->GetEdges()->SetTail(E);
						}
					}
				}

			} else {
				if (flag == false) { //seq
					dcTaskEdge* E = MyForkTask->GetEdges();
					while (E != NULL) {
						size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
						dcTask* MyTask = Clone(Tasks.at(index));
						Custom.push_back(MyTask);
						Seq.push_back(MyTask);

						dcTaskEdge *E2 = new dcTaskEdge();
						E2->SetType(1);
						E2->SetConnectTo(MyTask);

						if (E->GetConnectTo()->GetEdges() == 0) {
							E->GetConnectTo()->SetEdges(E);
							E->GetConnectTo()->GetEdges()->SetTail(E2);
						} else {
							E->GetConnectTo()->GetEdges()->GetTail()->SetNext(
									E2);
							E->GetConnectTo()->GetEdges()->SetTail(E2);
						}

						E = E->GetNext();
					}
					flag = true;
				} else {
					vector<dcTask*> Temp = Seq;
					Seq.clear();
					for (size_t i = 0; i < Temp.size(); i++) {
						size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
						dcTask* MyTask = Clone(Tasks.at(index));
						Custom.push_back(MyTask);
						Seq.push_back(MyTask);

						dcTaskEdge *E = new dcTaskEdge();
						E->SetType(1);
						E->SetConnectTo(MyTask);

						if (Temp.at(i)->GetEdges() == 0) {
							Temp.at(i)->SetEdges(E);
							Temp.at(i)->GetEdges()->SetTail(E);
						} else {
							Temp.at(i)->GetEdges()->GetTail()->SetNext(E);
							Temp.at(i)->GetEdges()->SetTail(E);
						}
					}

				}
			}
			//add individual tasks to custom workload
			for (int i = 0; i < 4; i++) {
				size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
				dcTask* IndTask = Clone(Tasks.at(index));
				Custom.push_back(IndTask);
			}
			count = count + 1;
		}

		CustomWorkloads.push_back(Custom);
	}

	if (Independent.size() > 0) {
		for (int i = 0; i < 1; i++) {
			vector<dcTask*> Tasks;
			Independents.push_back(Tasks);
		}

		unsigned int MaxInd = 4; //create max of 4 independent tasks
		if (Independent.size() > MaxInd) {

			for (unsigned int i = 0; i < MaxInd; i++) {
				int index = rand() % (MaxInd - 1 + 1) + 0;
				Independents.at(0).push_back(Clone(Independent.at(index)));
			}

		} else {

			for (size_t i = 0; i < Independent.size(); i++) {
				size_t index = rand() % (Independent.size() - 1 + 1) + 0;
				Independents.at(0).push_back(Clone(Independent.at(index)));
			}
		}
	}

	//assign attributes to workloads
	AssignAttributes(dcApplicationIn, Independents);
	AssignAttributes(dcApplicationIn, Further);
	AssignAttributes(dcApplicationIn, Fork);
	AssignAttributes(dcApplicationIn, Join);
	AssignAttributes(dcApplicationIn, Sequential);
	AssignAttributes(dcApplicationIn, Feedback);
	AssignAttributes(dcApplicationIn, CustomWorkloads);

	//CREATE MICROWORKLOADS

	vector<dcTaskGraph*> MicroIndependent = CreateMicroIndependent(
			Independents);
	vector<dcTaskGraph*> MicroFork = CreateMicroFork(Fork);
	vector<dcTaskGraph*> MicroJoin = CreateMicroJoin(Join);
	vector<dcTaskGraph*> MicroSequential = CreateMicroSequential(Sequential);
	vector<dcTaskGraph*> MicroFeedback = CreateMicroFeedback(Feedback);
	vector<dcTaskGraph*> MicroFurther = CreateMicroFurther(Further);
	vector<dcTaskGraph*> MicroCustom = CreateMicroCustom(CustomWorkloads);

	//PUT THE WORKLOADS TOGETHER
	vector<dcTaskGraph*> WorkLoads;
	WorkLoads.reserve(
			MicroIndependent.size() + MicroFork.size() + MicroJoin.size()
					+ MicroSequential.size() + MicroFeedback.size()
					+ MicroFurther.size() + MicroCustom.size());
	WorkLoads.insert(WorkLoads.end(), MicroIndependent.begin(),
			MicroIndependent.end());
	WorkLoads.insert(WorkLoads.end(), MicroFork.begin(), MicroFork.end());
	WorkLoads.insert(WorkLoads.end(), MicroJoin.begin(), MicroJoin.end());
	WorkLoads.insert(WorkLoads.end(), MicroSequential.begin(),
			MicroSequential.end());
	WorkLoads.insert(WorkLoads.end(), MicroFeedback.begin(),
			MicroFeedback.end());
	WorkLoads.insert(WorkLoads.end(), MicroFurther.begin(), MicroFurther.end());
	WorkLoads.insert(WorkLoads.end(), MicroCustom.begin(), MicroCustom.end());

	end = clock();
	cout << "Time to generate microworkload: "
			<< (double) (end - start) / CLOCKS_PER_SEC << " seconds." << "\n\n";
	cout << "Complete generating microworkloads: " << endl;

	return WorkLoads;

}

vector<pair<dcTask*, pair<string, int>>> dcApplication::AnalyseTask(dcTask* MyTask) {
	static queue <dcTask*> TaskQueue;
	vector<dcTask*> Children;

	//store result
	pair<dcTask*, pair<string, int>> Temp;

	if(MyTask->GetVisited() == 0) {
		MyTask->SetVisited(1);

		dcTaskEdge* MyTaskEdges = MyTask->GetEdges();
		while(MyTaskEdges != NULL) {
			if(MyTaskEdges->GetType() == 1) {
				Children.push_back(MyTaskEdges->GetConnectTo());
				if(MyTaskEdges->GetConnectTo()->GetVisited() == 0) {
					TaskQueue.push(MyTaskEdges->GetConnectTo());
				}
			}
			MyTaskEdges = MyTaskEdges->GetNext();
		}

		//check the number of back edges the task has
		dcTaskEdge* MyBackEdge = MyTask->GetEdges();
		size_t counter = 0;
		while(MyBackEdge != NULL) {
			bool EdgeType = MyBackEdge->GetType();
			if(EdgeType == 0) {
				counter = counter + 1;
			}

			MyBackEdge = MyBackEdge->GetNext();
		}

		string strTemp = "";
		int NoOfChildren = Children.size();
		if(NoOfChildren == 0 && counter == 1) {
			strTemp = "I";
		} else if(NoOfChildren == 1) {
			strTemp = "S";
		} else if(NoOfChildren > 1) {
			strTemp = "F";
		}
		if(counter > 1) {
			strTemp = "J";
		}

		if(strTemp != "") {
			pair<string , int> Values;
			Values.first = strTemp;
			if(strTemp == "J") {
				Values.second = counter;
			} else {
				Values.second = NoOfChildren;
			}

			Temp.first=MyTask;
			Temp.second=Values;
			Result.push_back(Temp);
		}

	}

	while(!TaskQueue.empty()) {
		dcTask* MyTask = TaskQueue.front();
		TaskQueue.pop();
		AnalyseTask(MyTask);
	}

	//do some refinement
	string temp = "J";
	dcTask* JoinTask = NULL;
	for(size_t i = 0; i < Result.size(); i++) {
		if(Result.at(i).second.first == temp.c_str()) {
			JoinTask = Result.at(i).first;
		}

		unsigned int j = 0;
		while(j < Result.size()) {
			dcTaskEdge* Edges = Result.at(j).first->GetEdges();
			while(Edges != NULL) {
				if(Edges->GetType() == 1) {
					if(Edges->GetConnectTo() == JoinTask) {
						Result.erase(Result.begin()+j);
					}
				}
				Edges = Edges->GetNext();
			}
			j++;
		}
	}
	return Result;
}
void dcApplication::AnalyzeGraph_(dcTaskGraph* TaskGraphIn) { //print out
	dcTask *MyTask = TaskGraphIn->GetHead();
	if (MyTask == NULL) {
		cout << "Task :" << MyTask->GetName() << " not found in dcGraph "
				<< endl;
	} else {
		while (MyTask != NULL) {
			cout << MyTask->GetName() << "->";

			dcTaskEdge* Edge = MyTask->GetEdges();
			while (Edge != NULL) {
				if (Edge->GetType() == 1) { //forward edge
					cout << Edge->GetConnectTo()->GetName() << " ";
				}

				Edge = Edge->GetNext();
			}

			MyTask = MyTask->GetNext();
			cout << endl;

		}
	}
	cout << "======================================================" << endl;
}
vector<dcActEvent*> dcApplication::GetPeriodicEvents(
		AmApplication* dcApplicationIn) {
	return dcApplicationIn->GetPeriodicEvents();
}
vector<dcActEvent*> dcApplication::GetSporadicEvents(
		AmApplication* dcApplicationIn) {
	return dcApplicationIn->GetSporadicEvents();
}
vector<dcActEvent*> dcApplication::GetAperiodicEvent(
		AmApplication* dcApplicationIn) {
	return dcApplicationIn->GetAperiodicEvent();
}
vector<dcActEvent*> dcApplication::GetDeterministicIrregular(
		AmApplication* dcApplicationIn) {
	return dcApplicationIn->GetDeterministicIrregular();
}
vector<dcActEvent*> dcApplication::GetInterprocessEvent(
		AmApplication* dcApplicationIn) {
	return dcApplicationIn->GetInterprocessEvent();
}
//micro bench mark task graph
dcTaskGraph* dcApplication::CreateGraph() {
	dcTaskGraph* TaskGraphIn = new dcTaskGraph();
	return TaskGraphIn;
}

void dcApplication::AddTasks(dcTaskGraph* TaskGraphIn, dcTask* TaskIn) {
	if (TaskGraphIn->GetHead() == NULL && TaskGraphIn->GetTail() == NULL) {
		TaskGraphIn->SetHead(TaskIn);
		TaskGraphIn->SetTail(TaskIn);
	} else {
		TaskGraphIn->GetTail()->SetNext(TaskIn);
		TaskGraphIn->SetTail(TaskIn);
	}
}
vector<dcTask*> dcApplication::GetSequence(dcTask* MyTask) {
	static queue<dcTask*> TaskQueue;
	vector<dcTask*> Children;

	if (MyTask->GetVisited() == 0) {
		dcTask* Temp = MyTask;
		Result2.push_back(Temp);
		MyTask->SetVisited(1);

		dcTaskEdge* MyTaskEdges = MyTask->GetEdges();
		while (MyTaskEdges != NULL) {
			if (MyTaskEdges->GetType() == 1) {
				if (MyTaskEdges->GetConnectTo()->GetVisited() == 0) {
					Children.push_back(MyTaskEdges->GetConnectTo());
				}
			}
			MyTaskEdges = MyTaskEdges->GetNext();
		}
		if (Children.size() == 1) {
			if (Children.front() != FirstTask) {
				TaskQueue.push(Children.front());
			}
		}
	}
	while (!TaskQueue.empty()) {
		dcTask* MyTask = TaskQueue.front();
		TaskQueue.pop();
		GetSequence(MyTask);
	}

	return Result2;
}

dcTask* dcApplication::Clone(dcTask* TaskIn) {
	dcTask* Task = new dcTask(TaskIn->GetName());
	Task->SetTag(TaskIn->GetTag());
	Task->SetPriority(TaskIn->GetPriority());
	Task->SetMultipleTaskActivationLimit(
			TaskIn->GetMultipleTaskActivationLimit());
	Task->SetPreemption(TaskIn->GetPreemption());

	string Unit = TaskIn->GetDeadlineUnit();
	int Value = TaskIn->GetDeadlineValue();
	if (Unit != "") {
		Unit = "ps";
	}
	pair<int, string> Deadline;
	Deadline.first = Value;
	Deadline.second = Unit;

	Task->SetDeadline(Deadline);
	Task->SetActEvent(TaskIn->GetActEvent());
	Task->SetSize(TaskIn->GetSize());
	Task->SetResources(TaskIn->GetResources());
	Task->SetID(TaskIn->GetID());
	Task->SetEdges(NULL);
	Task->SetNext(NULL);
	Task->SetRunnables(TaskIn->GetRunnableCalls());
	return Task;
}

bool dcApplication::Existing(size_t size, vector<vector<dcTask*>> PatternIn) {
	bool Exist = 0;
	for (size_t i = 0; i < PatternIn.size(); i++) {
		if (size == PatternIn.at(i).size()) {
			Exist = 1;
			break;
		}
	}

	return Exist;
}

bool dcApplication::IsExisting(vector<dcTask*> TasksIn, dcTask* TaskIn) {
	bool Existing = 0;
	for (size_t i = 0; i < TasksIn.size(); i++) {
		if (TaskIn->GetID() == TasksIn.at(i)->GetID()) {
			Existing = 1;
			break;
		}
	}

	return Existing;
}

vector<vector<int>> dcApplication::Partition(vector<int> vectorIn) {
	vector<vector<int> > left, right, result;
	vector<int> l, r;

	int first = vectorIn.front();
	bool same = 1;
	for (size_t i = 0; i < vectorIn.size(); i++) {
		if (i != 0) {
			if (vectorIn.at(i) != first) {
				same = 0;
				break;
			}
		}
	}

	if (vectorIn.size() <= 4 || same == 1) {
		result.push_back(vectorIn);
		return result;
	}
	double average = 0;
	for (size_t i = 0; i < vectorIn.size(); i++)
		average += vectorIn[i];
	average = average / (vectorIn.size());
	for (size_t i = 0; i < vectorIn.size(); i++) {
		double num = (double) (vectorIn[i] * 1.0);
		if (num > average)
			r.push_back(vectorIn[i]);
		else
			l.push_back(vectorIn[i]);
	}
	left = Partition(l);
	right = Partition(r);

	for (size_t i = 0; i < left.size(); i++)
		result.push_back(left[i]);
	for (size_t i = 0; i < right.size(); i++)
		result.push_back(right[i]);
	return result;
}

bool dcApplication::IsIn(vector<int> Vals, int Val) {
	bool In = false;
	for (size_t i = 0; i < Vals.size(); i++) {
		if (Vals.at(i) == Val) {
			In = true; // is int
			break;
		}
	}
	return In;
}

bool dcApplication::IsVisted(vector<string> Visited, string strIn) {
	bool Visit = 0;
	for (size_t i = 0; i < Visited.size(); i++) {
		string temp = Visited.at(i);
		transform(strIn.begin(), strIn.end(), strIn.begin(), ::tolower);
		transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
		if (temp == strIn) {
			Visit = 1;
			break;
		}
	}

	return Visit;
}

void dcApplication::AssignAttributes(AmApplication* dcApplicationIn,
		vector<vector<dcTask*>> Worloads) {
	//PREEMPTIVE INFO

	int Preemptive = dcApplicationIn->GetNoOfPreemptive();
	int NonPreemptive = dcApplicationIn->GetNoOfNonPreemptive();

	size_t counter = 0;
	while (counter < Worloads.size()) {
		if (Preemptive != 0 && NonPreemptive != 0) {
			unsigned int NoOfTasks = Worloads.at(counter).size();
			int AVG = (int) NoOfTasks / 2;
			int MIN = AVG + 1;
			int MAX = NoOfTasks;
			srand((unsigned int) time(NULL));
			int ValA = rand() % (MAX - MIN + 1) + MIN;
			int ValB = NoOfTasks - ValA;

			if (std::max(Preemptive, NonPreemptive) == Preemptive) { // more preemptive (VAL A)
				vector<string> Premp;
				int c = 0;
				if (ValA != 0) {
					while (c < ValA) {
						Premp.push_back("preemptive");
						c++;
					}
				}

				c = 0;
				if (ValB != 0) {
					while (c < ValB) {
						Premp.push_back("non-preemptive");
						c++;
					}
				}

				vector<int> Visited;
				while (Visited.size() != NoOfTasks) {
					int Position = rand() % ((MAX - 1) - 0 + 1) + 0;

					if (!Visited.empty()) {
						bool In = IsIn(Visited, Position);
						if (In == false) {
							Visited.push_back(Position);
							Worloads.at(counter).at(Position)->SetPreemption(
									Premp.at(Position));
						}
					} else {
						Visited.push_back(Position);
						Worloads.at(counter).at(Position)->SetPreemption(
								Premp.at(Position));
					}
				}

			} else { //less preemptive
				vector<string> Premp;
				int c = 0;
				if (ValA != 0) {
					while (c < ValA) {
						Premp.push_back("non-preemptive");
						c++;
					}
				}

				c = 0;
				if (ValB != 0) {
					while (c < ValB) {
						Premp.push_back("preemptive");
						c++;
					}
				}

				vector<int> Visited;
				while (Visited.size() != NoOfTasks) {
					int Position = rand() % (MAX - 1 + 1) + 0;

					if (!Visited.empty()) {
						bool In = IsIn(Visited, Position);
						if (In == false) {
							Visited.push_back(Position);
							Worloads.at(counter).at(Position)->SetPreemption(
									Premp.at(Position));
						}
					} else {
						Visited.push_back(Position);
						Worloads.at(counter).at(Position)->SetPreemption(
								Premp.at(Position));
					}
				}

			}

		}

		//DEADLINE INFO
		vector<int> DeadlineAttributes = dcApplicationIn->GetDeadlines();
		if (DeadlineAttributes.size() > 0) {
			if (DeadlineAttributes.size() >= Worloads.at(counter).size()) {
				//Partition deadline
				vector<vector<int>> PartitionedVal = Partition(
						DeadlineAttributes, Worloads.at(counter).size());

				//Randomly (take random deadline from each partition) assign deadline attribute to each task
				for (vector<dcTask*>::size_type i = 0;
						i < Worloads.at(counter).size(); i++) {
					int Position = rand()
							% (PartitionedVal.at(i).size() - 1 + 1) + 0;
					string DeadlineUnit =
							Worloads.at(counter).at(i)->GetDeadlineUnit();
					Worloads.at(counter).at(i)->SetDeadline(
							pair<int, string>(PartitionedVal.at(i).at(Position),
									DeadlineUnit));
				}

			} else {
				//Here the number of tasks is greater than number of deadline attributes
				unsigned int j = 0;
				for (vector<dcTask*>::size_type i = 0;
						i < Worloads.at(counter).size(); i++) {
					string DeadlineUnit =
							Worloads.at(counter).at(j)->GetDeadlineUnit();
					int DeadlineValue = DeadlineAttributes.at(j);
					dcTask* Task = Worloads.at(counter).at(j);
					Task->SetDeadline(
							pair<int, string>(DeadlineValue, DeadlineUnit));

					//Worloads.at(counter).at(j)->SetDeadline(pair<int, string>(DeadlineAttributes.at(i), DeadlineUnit));

					if (DeadlineAttributes.size() - 1 == j) {
						j = 0;
					} else {
						j++;
					}
				}
			}
		}

		//priority info
		vector<int> PriorityAttributes = dcApplicationIn->GetPriorities();
		if (PriorityAttributes.size() > 0) {
			if (PriorityAttributes.size() >= Worloads.at(counter).size()) {
				//Partition deadline
				vector<vector<int>> PartitionedVal = Partition(
						PriorityAttributes, Worloads.at(counter).size());

				//Randomly (take random priority from each partition) assign priority attribute to each task
				for (vector<dcTask*>::size_type i = 0;
						i < Worloads.at(counter).size(); i++) {
					int Position = rand()
							% (PartitionedVal.at(i).size() - 1 + 1) + 0;
					Worloads.at(counter).at(i)->SetPriority(
							PartitionedVal.at(i).at(Position));
				}

			} else {
				//Here the number of tasks is greater than number of priority attributes
				unsigned int j = 0;
				for (vector<dcTask*>::size_type i = 0;
						i < Worloads.at(counter).size(); i++) {
					Worloads.at(counter).at(i)->SetPriority(
							PriorityAttributes.at(j));

					if (PriorityAttributes.size() - 1 == j) {
						j = 0;
					} else {
						j++;
					}
				}
			}
		}
		counter++;
	}
}

vector<dcTaskGraph*> dcApplication::CreateMicroIndependent(
		vector<vector<dcTask*>> Independents) {
	vector<dcTaskGraph*> MicroIndependent;
	size_t count = 0;
	while (count < Independents.size()) {
		dcTaskGraph* Graph = CreateGraph();
		for (size_t i = 0; i < Independents.at(count).size(); i++) {
			dcTask* Temp = Independents.at(count).at(i);
			Temp->SetNext(0);
			AddTasks(Graph, Temp);
		}
		MicroIndependent.push_back(Graph);
		count++;
	}
	return MicroIndependent;
}
vector<dcTaskGraph*> dcApplication::CreateMicroFork(
		vector<vector<dcTask*>> Fork) {
	vector<dcTaskGraph*> MicroFork;
	//fork
	size_t count = 0;
	while (count < Fork.size()) {
		dcTaskGraph* Graph = CreateGraph();
		for (size_t i = 0; i < Fork.at(count).size(); i++) {
			dcTask* Temp = Fork.at(count).at(i);
			Temp->SetNext(0);
			AddTasks(Graph, Temp);
		}
		MicroFork.push_back(Graph);
		count++;
	}
	return MicroFork;
}
vector<dcTaskGraph*> dcApplication::CreateMicroJoin(
		vector<vector<dcTask*>> Join) {
	vector<dcTaskGraph*> MicroJoin;
	size_t count = 0;

	//join
	while (count < Join.size()) {
		dcTaskGraph* Graph = CreateGraph();
		for (int i = (Join.at(count).size() - 1); i >= 0; i--) {
			dcTask* Temp = Join.at(count).at(i);
			Temp->SetNext(0);
			AddTasks(Graph, Temp);
		}
		MicroJoin.push_back(Graph);
		count++;
	}
	return MicroJoin;
}
vector<dcTaskGraph*> dcApplication::CreateMicroSequential(
		vector<vector<dcTask*>> Sequential) {
	vector<dcTaskGraph*> MicroSequential;
	size_t count = 0;

	while (count < Sequential.size()) {
		dcTaskGraph* Graph = CreateGraph();
		for (size_t i = 0; i < Sequential.at(count).size(); i++) {
			dcTask* Task = Clone(Sequential.at(count).at(i));
			dcTask* Dest = NULL;
			bool valid = 0;

			if (i != (Sequential.at(count).size() - 1)) {
				int j = i + 1;
				Dest = Clone(Sequential.at(count).at(j));
				valid = 1;
			}
			if (valid == 1) {
				dcTaskEdge *E = new dcTaskEdge();
				E->SetType(1);
				E->SetNext(0);
				E->SetConnectTo(Dest);
				if (Task->GetEdges() == 0) {
					Task->SetEdges(E);
					Task->GetEdges()->SetTail(E);
				} else {
					Task->GetEdges()->GetTail()->SetNext(E);
					Task->GetEdges()->SetTail(E);
				}
			}
			AddTasks(Graph, Task);
		}
		MicroSequential.push_back(Graph);
		count++;
	}
	return MicroSequential;
}
vector<dcTaskGraph*> dcApplication::CreateMicroFeedback(
		vector<vector<dcTask*>> Feedback) {
	vector<dcTaskGraph*> MicroFeedback;
	size_t count = 0;
	while (count < Feedback.size()) {
		dcTaskGraph* Graph = CreateGraph();
		for (size_t i = 0; i < Feedback.at(count).size(); i++) {
			dcTask* Task = Feedback.at(count).at(i);
			dcTask* Dest = NULL;
			bool valid = 0;

			if (i != (Feedback.at(count).size() - 1)) {
				int j = i + 1;
				Dest = Feedback.at(count).at(j);
				valid = 1;
			}
			if (valid == 1) {
				dcTaskEdge *E = new dcTaskEdge();
				E->SetType(1);
				E->SetNext(0);
				E->SetConnectTo(Dest);
				if (Task->GetEdges() == 0) {
					Task->SetEdges(E);
					Task->GetEdges()->SetTail(E);
				} else {
					Task->GetEdges()->GetTail()->SetNext(E);
					Task->GetEdges()->SetTail(E);
				}
			}
			AddTasks(Graph, Task);
		}
		MicroFeedback.push_back(Graph);
		count++;
	}
	return MicroFeedback;
}
vector<dcTaskGraph*> dcApplication::CreateMicroFurther(
		vector<vector<dcTask*>> Further) {
	//graphs for further micro-workloads
	vector<dcTaskGraph*> MicroFurther;
	size_t count = 0;
	while (count < Further.size()) {
		dcTaskGraph* Graph = CreateGraph();
		for (size_t i = 0; i < Further.at(count).size(); i++) {
			dcTask* Temp = Further.at(count).at(i);
			Temp->SetNext(0);
			AddTasks(Graph, Temp);
		}
		MicroFurther.push_back(Graph);
		count++;
	}
	return MicroFurther;
}
vector<dcTaskGraph*> dcApplication::CreateMicroCustom(
		vector<vector<dcTask*>> Custom) {
	//graphs for further micro-workloads
	vector<dcTaskGraph*> MicroCustom;
	size_t count = 0;
	while (count < Custom.size()) {
		dcTaskGraph* Graph = CreateGraph();
		for (size_t i = 0; i < Custom.at(count).size(); i++) {
			dcTask* Temp = Custom.at(count).at(i);
			Temp->SetNext(0);
			AddTasks(Graph, Temp);
		}
		MicroCustom.push_back(Graph);
		count++;
	}
	return MicroCustom;
}
vector<int> dcApplication::SelectionSort(vector<int> input) {
	vector<int> input_temp = input;
	for (size_t i = 0; i < input_temp.size() - 1; ++i) {
		int iSmallest = i;
		for (size_t j = i + 1; j < input_temp.size(); ++j) {
			if (input_temp.at(iSmallest) > input_temp.at(j)) {
				iSmallest = j;
			}
		}
		int iSwap = input_temp.at(iSmallest);
		input_temp.at(iSmallest) = input_temp.at(i);
		input_temp.at(i) = iSwap;

	}
	return input_temp;
}

/*
 //get runnables computation instructions
 vector<pair<string, pair<int, int>>> dcApplication::GetAllRunnableComputation(dcTaskGraph* dcTaskGraphIn){
 vector<pair<string, pair<int, int>>> RunComputation;
 int counter = 0;

 FILE *exeInst;
 //exeInst = fopen ("./../Power_Estimator/Instruction_fixed_Power.txt","w+");
 exeInst = fopen ("./../Energy_Estimator/Instruction_fixed_Power.txt","w+");
 //fputs("Runnable ID; Index; Cycles \n", exeInst);

 vector<dcRunnable*> Runnables = GetAllRunnables(dcTaskGraphIn);
 for(size_t i=0; i < Runnables.size(); i++){
 pair<string, pair<int, int>> Temp;

 int InstSize = Runnables.at(i)->GetAllInstructions().size();

 for(size_t j=0; j < InstSize; j++){
 //get the type of instruction
 string InstrName = Runnables.at(i)->GetAllInstructions().at(j)->GetName();
 if(InstrName == "sw:InstructionsConstant"){
 dcInstruction* inst = Runnables.at(i)->GetAllInstructions().at(j);
 dcExecutionCyclesConstantInstruction* einst = static_cast<dcExecutionCyclesConstantInstruction*>(inst);
 if(einst){
 counter++;
 Temp.first = Runnables.at(i)->GetName();
 Temp.second.first = counter;
 Temp.second.second = einst->GetValue();


 string counterS = static_cast<ostringstream*>( &(ostringstream() << (Temp.second.first)) )->str();
 string cyclesS = static_cast<ostringstream*>( &(ostringstream() << (Temp.second.second)) )->str();
 string exeInstS = Temp.first + " ," + counterS +" ,"+ cyclesS + "\n";
 const char* exeInstS_const = exeInstS.c_str();
 fputs(exeInstS_const, exeInst);

 }
 }

 if(Temp.first != ""){
 RunComputation.push_back(Temp);
 }
 }
 counter = 0;
 }

 return RunComputation;
 }
 */

// Added 02_09_2015
//Get informations about Runnables
void dcApplication::dumpRunnablesToFiles(dcTaskGraph* dcTaskGraphIn,
		string outputFolder) {
	FILE *exeInst;
	int N_flt_pack = 32;
	int N_bytes_flit = 1;
	exeInst = fopen((outputFolder + "/Runnables.txt").c_str(), "w+");

	vector<dcRunnableCall*> Runnables = GetAllRunnables(dcTaskGraphIn);
	for (size_t i = 0; i < Runnables.size(); i++) {

		vector<dcInstruction*>::size_type InstSize =
				Runnables.at(i)->GetAllInstructions().size();
		string Runnable_Name = Runnables.at(i)->GetRunClassName();
		string Runnable_Id = Runnables.at(i)->GetRunClassId();
		int inst_count = 0;
		int label_access_count = 0;
		int packets = 0;
		int size_Label = 0;

		for (vector<dcInstruction*>::size_type j = 0; j < InstSize; j++) {
			//get the type of instruction
			string InstrName =
					Runnables.at(i)->GetAllInstructions().at(j)->GetName();
			dcInstruction* inst = Runnables.at(i)->GetAllInstructions().at(j);

			if ((InstrName == "sw:InstructionsConstant")
					|| (InstrName == "sw:InstructionsDeviation")) {
				dcExecutionCyclesConstantInstruction* einst_c =
						dynamic_cast<dcExecutionCyclesConstantInstruction*>(inst);
				dcExecutionCyclesDeviationInstruction* einst_d =
						dynamic_cast<dcExecutionCyclesDeviationInstruction*>(inst);
				if (einst_c || einst_d) {
					inst_count++;
				}
			}

			if (InstrName == "sw:LabelAccess") {
				dcRemoteAccessInstruction *rinst =
						static_cast<dcRemoteAccessInstruction*>(inst);
				if (rinst) {
					label_access_count++;

					//Compute here the number of packets
					if (!(rinst->GetWrite())) {
						packets++;	  //Read access
					}

					size_Label = rinst->GetLabel()->GetSize();
					packets = packets
							+ (size_Label / (N_flt_pack * 8 * N_bytes_flit));
					if ((size_Label % (N_flt_pack * 8 * N_bytes_flit)) > 0) {
						packets = packets + 1;
					}
				}
			}
		}

		string Inst_str = static_cast<ostringstream*>(&(ostringstream()
				<< (inst_count)))->str();
		string Label_str = static_cast<ostringstream*>(&(ostringstream()
				<< (label_access_count)))->str();
		string Packets = static_cast<ostringstream*>(&(ostringstream()
				<< (packets)))->str();
		string exeInstS = Runnable_Id + " " + Runnable_Name + " " + Inst_str
				+ " " + Label_str + " " + Packets + "\n";
		const char* exeInstS_const = exeInstS.c_str();
		fputs(exeInstS_const, exeInst);
	}
	fclose(exeInst);
}

vector<vector<int> > dcApplication::Partition(vector<int> input, int num) {
	vector<vector<int> > result;
	vector<int> temp = SelectionSort(input);
	int num_temp = num;

	for (int i = 0; i < num; i++) {

		int avg = (int) (floor(temp.size() / num_temp));
		vector<int> rst_temp;

		for (int j = 0; j < avg; j++) {
			rst_temp.push_back(temp.at(j));
		}

		temp.erase(temp.begin() + 0, temp.begin() + avg);

		result.push_back(rst_temp);

		num_temp--;
	}

	return result;
}

void dcApplication::dumpLabelAccesses(dcTaskGraph* dcTaskGraphIn,
		string outputFolder) {
	int counter = 0;
	FILE *commInst = fopen((outputFolder + "/Label_accesses_Power.txt").c_str(),
			"w+");
	vector<dcRunnableCall*> Runnables = GetAllRunnables(dcTaskGraphIn);
	for (size_t i = 0; i < Runnables.size(); i++) {
		pair<pair<string, int>, pair<int, int>> Temp;
		vector<dcInstruction*>::size_type InstSize =
				Runnables.at(i)->GetAllInstructions().size();
		for (vector<dcInstruction*>::size_type j = 0; j < InstSize; j++) {
			//get the type of instruction
			string InstrName =
					Runnables.at(i)->GetAllInstructions().at(j)->GetName();
			if (InstrName == "sw:LabelAccess") {
				dcInstruction* inst = Runnables.at(i)->GetAllInstructions().at(
						j);
				dcRemoteAccessInstruction *rinst =
						static_cast<dcRemoteAccessInstruction*>(inst);
				if (rinst) {
					counter++;
					Temp.first.first = Runnables.at(i)->GetRunClassName();
					if (!rinst->GetWrite()) {
						Temp.first.second = 0;
					} else {
						Temp.first.second = 1;
					}
					Temp.second.first = counter;
					Temp.second.second = rinst->GetLabel()->GetSize();
					string counterS =
							static_cast<ostringstream*>(&(ostringstream()
									<< (Temp.second.first)))->str();
					string sizeS = static_cast<ostringstream*>(&(ostringstream()
							<< (Temp.second.second)))->str();
					string RdWr = static_cast<ostringstream*>(&(ostringstream()
							<< (Temp.first.second)))->str();
					string comInstS = Temp.first.first + " ," + counterS + " ,"
							+ sizeS + " ," + RdWr + "\n";
					const char* comInstS_const = comInstS.c_str();
					fputs(comInstS_const, commInst);
				}
			}
		}
		counter = 0;
	}
}

void dcApplication::dumpLabelAccessesPerRunnable(dcTaskGraph* dcTaskGraphIn,
		string outputFolder) {
	ofstream os;
	os.open(outputFolder + "/RunnablesCommunication.csv");
	os << "Name" << ",";
	os << "Id" << ",";
	os << "Nb read instructions" << ",";
	os << "Total read size" << ",";
	os << "Nb write instructions" << ",";
	os << "Total write size" << ",";
	os << "Nb read/write instructions" << ",";
	os << "Total read/write size";
	os << endl;
	vector<dcRunnableCall*> runnables = GetAllRunnables(dcTaskGraphIn);
	for (size_t i = 0; i < runnables.size(); i++) {
		dcRunnableCall* runCall = runnables.at(i);
		int nbWrites = 0;
		int nbWritesBytes = 0;
		int nbReads = 0;
		int nbReadsBytes = 0;
		vector<dcInstruction*> insts = runnables.at(i)->GetAllInstructions();
		for (vector<dcInstruction*>::size_type j = 0; j < insts.size(); j++) {
			dcInstruction* inst = insts.at(j);
			if (inst->GetName() == "sw:LabelAccess") {
				dcRemoteAccessInstruction *rinst =
						static_cast<dcRemoteAccessInstruction*>(inst);
				if (rinst->GetWrite()) {
					nbWrites++;
					nbWritesBytes += rinst->GetLabel()->GetSize();
				} else {
					nbReads++;
					nbReadsBytes += rinst->GetLabel()->GetSize();
				}
			}
		}
		os << runCall->GetRunClassName() << ",";
		os << runCall->GetRunClassId() << ",";
		os << nbReads << ",";
		os << nbReadsBytes << ",";
		os << nbWrites << ",";
		os << nbWritesBytes << ",";
		os << (nbReads + nbWrites) << ",";
		os << (nbReadsBytes + nbWritesBytes);
		os << endl;
	}
	os.close();
}

void dcApplication::dumpTaskGraphFile(string file, vector<dcTask *> tasks) {

	FILE* dcGraphDotFile = fopen(file.c_str(), "w+");
	fputs("digraph application {\n", dcGraphDotFile);
	for (std::vector<dcTask *>::iterator it = tasks.begin(); it != tasks.end();
			++it) {
		fputs((*it)->GetName().c_str(), dcGraphDotFile);
		dcActEvent* event = (*it)->GetActEvent();
		if (event != NULL) {
			if (event->GetType() == "stimuli:Periodic") {
				dcPeriodicEvent* periodic = static_cast<dcPeriodicEvent*>(event);
				pair<int, string> recurrence = periodic->GetRecurrence();
				fputs("[shape=rect,color=blue,fontcolor=blue,label=\"",
						dcGraphDotFile);
				fputs((*it)->GetName().c_str(), dcGraphDotFile);
				fprintf(dcGraphDotFile, "\\nPeriod=%d%s", recurrence.first,
						recurrence.second.c_str());
				fputs("\"]", dcGraphDotFile);
			} else if (event->GetType() == "stimuli:Sporadic") {
				dcSporadicEvent* sporadic = static_cast<dcSporadicEvent*>(event);
				double lowerBound = sporadic->GetLowerBound();
				double upperBound = sporadic->GetUpperBound();
				fputs("[shape=rect,color=red,fontcolor=red,label=\"",
						dcGraphDotFile);
				fputs((*it)->GetName().c_str(), dcGraphDotFile);
				fprintf(dcGraphDotFile, "\\nSporadic=%0.2f,%0.2f", lowerBound,
						upperBound);
				fputs("\"]", dcGraphDotFile);
			} else {
				fputs("[shape=rect,color=black,fontcolor=black,label=\"",
						dcGraphDotFile);
				fputs((*it)->GetName().c_str(), dcGraphDotFile);
				fputs("\\nNon Periodic\"]", dcGraphDotFile);
			}
		}
		fputs(";\n", dcGraphDotFile);
	}
	for (std::vector<dcTask *>::iterator it = tasks.begin(); it != tasks.end();
			++it) {
		dcTaskEdge * edge = (*it)->GetEdges();
		while (edge != NULL) {
			if (edge->GetType()) {
				fputs((*it)->GetName().c_str(), dcGraphDotFile);
				fputs(" -> ", dcGraphDotFile);
				fputs(edge->GetConnectTo()->GetName().c_str(), dcGraphDotFile);
				fputs(";\n", dcGraphDotFile);
			}
			edge = edge->GetNext();
		}
	}
	fputs("}", dcGraphDotFile);
	fclose(dcGraphDotFile);
}

void dcApplication::dumpTaskAndRunnableGraphFile(string file,
		vector<dcTask *> tasks) {
	ofstream dcGraphDotFile(file);
	dcGraphDotFile << "digraph Runnable {" << endl;
	dcGraphDotFile << "\tcompound=true;" << endl;
	unsigned int i = 0;
	for (std::vector<dcTask *>::iterator it = tasks.begin(); it != tasks.end();
			++it) {
		dcGraphDotFile << "\tsubgraph cluster" << i << " {" << endl;
		dcActEvent* event = (*it)->GetActEvent();
		string color;
		ostringstream period;
		period << " - ";
		if (event != NULL) {
			if (event->GetType() == "stimuli:Periodic") {
				color = "blue";
				dcPeriodicEvent* periodic = static_cast<dcPeriodicEvent*>(event);
				pair<int, string> recurrence = periodic->GetRecurrence();
				period << "Period=" << recurrence.first << recurrence.second;
			} else if (event->GetType() == "stimuli:Sporadic") {
				color = "red";
				dcSporadicEvent* sporadic = static_cast<dcSporadicEvent*>(event);
				double lowerBound = sporadic->GetLowerBound();
				double upperBound = sporadic->GetUpperBound();
				period << "Sporadic=" << lowerBound << "," << upperBound;
			} else {
				color = "black";
				period << "Non periodic";
			}
		}
		dcGraphDotFile << "\t\tcolor=" << color << ";" << endl;
		dcGraphDotFile << "\t\tlabel=<<font color=\"" << color << "\">"
				<< (*it)->GetName() << period.str() << "</font>>;" << endl;
		dcRunnableCall *run = (*it)->GetRunnableCalls();
		while (run != NULL) {
			dcGraphDotFile << "\t\t" << run->GetRunClassName();
			dcGraphDotFile << "[label=\"";
			dcGraphDotFile << run->GetRunClassName();
			//dcGraphDotFile << "\\nWave id=" << hex << run->GetWaveID();
			dcGraphDotFile << "\"];" << endl;
			dcRunnableEdge *edge = run->GetEdges();
			while (edge != NULL) {
				if (edge->GetType() == 1) {
					dcGraphDotFile << "\t\t" << run->GetRunClassName() << " -> "
							<< edge->GetConnectTo()->GetRunClassName() << ";"
							<< endl;
				}
				edge = edge->GetNext();
			}
			run = run->GetNext();
		}
		i++;
		dcGraphDotFile << "\t}" << endl;
	}
	dcGraphDotFile << "}";
	dcGraphDotFile.close();
}

vector<dcRunnableCall*> dcApplication::GetIndependentNonPeriodicRunnables(
		dcTaskGraph* dcTaskGraphIn) {
	dcTask* MyTask = dcTaskGraphIn->GetHead();
	vector<dcRunnableCall*> independentRunnables;
	if (MyTask == 0) {
		cout << "No tasks in the dcApplication :(" << endl;
	} else {
		while (MyTask != 0) {
			dcRunnableCall* MyRunnables = MyTask->GetRunnableCalls();
			while (MyRunnables != NULL) {
				if (MyRunnables->GetPeriodInNano() == 0) {
					dcRunnableEdge* MyRunEdges = MyRunnables->GetEdges();
					bool isIndependent = true;
					while (MyRunEdges != NULL) {
						if (MyRunEdges->GetType() == 0) {
							isIndependent = false;
							break;
						}
						MyRunEdges = MyRunEdges->GetNext();
					}
					if (isIndependent) {
						independentRunnables.push_back(MyRunnables);
					}
				}
				MyRunnables = MyRunnables->GetNext();
			}
			MyTask = MyTask->GetNext();
		}
	}
	return independentRunnables;
}

vector<dcRunnableCall*> dcApplication::GetIndependentRunnables(
		dcTaskGraph* dcTaskGraphIn) {
	dcTask* MyTask = dcTaskGraphIn->GetHead();
	vector<dcRunnableCall*> independentRunnables;
	if (MyTask == 0) {
		cout << "No tasks in the dcApplication :(" << endl;
	} else {
		while (MyTask != 0) {
			dcRunnableCall* MyRunnables = MyTask->GetRunnableCalls();
			while (MyRunnables != NULL) {
				dcRunnableEdge* MyRunEdges = MyRunnables->GetEdges();
				bool isIndependent = true;
				while (MyRunEdges != NULL) {
					if (MyRunEdges->GetType() == 0) {
						isIndependent = false;
						break;
					}
					MyRunEdges = MyRunEdges->GetNext();
				}
				if (isIndependent) {
					independentRunnables.push_back(MyRunnables);
				}
				MyRunnables = MyRunnables->GetNext();
			}
			MyTask = MyTask->GetNext();
		}
	}
	return independentRunnables;
}

vector<dcRunnableCall*> dcApplication::GetPeriodicAndSporadicRunnables(
		dcTaskGraph* dcTaskGraphIn, bool seqDep) {
	dcTask* task = dcTaskGraphIn->GetHead();
	vector<dcRunnableCall*> runnables;
	if (task == 0) {
		cout << "No tasks in the dcApplication :(" << endl;
	} else {
		while (task != 0) {
			dcRunnableCall* runnable = task->GetRunnableCalls();
			while (runnable != NULL) {
				dcActEvent* event = runnable->GetActEvent();
				if (event != NULL) {
					string eventType = event->GetType();
					if (eventType == "stimuli:Periodic"
							|| eventType == "stimuli:Sporadic") {
						if (seqDep) {
							dcRunnableEdge* edge = runnable->GetEdges();
							bool independent = true;
							while (edge != NULL) {
								if (edge->GetType() == 0) {
									independent = false;
									break;
								}
								edge = edge->GetNext();
							}
							if (independent) {
								runnables.push_back(runnable);
							}
						} else {
							runnables.push_back(runnable);
						}
					}
				}
				runnable = runnable->GetNext();
			}
			task = task->GetNext();
		}
	}
	return runnables;
}

unsigned long int dcApplication::GetHyperPeriodWithOffset(dcTaskGraph* graph) {
	vector<dcRunnableCall*> periodicRunnables = GetPeriodicAndSporadicRunnables(
			graph, true);
	if (periodicRunnables.empty()) {
		return 0;
	}
	set<unsigned long> periods;
	unsigned long int maxOffset = 0;
	for (set<unsigned long>::size_type i = 0; i < periodicRunnables.size();
			++i) {
		periods.insert(periodicRunnables.at(i)->GetPeriodInNano());
		if (periodicRunnables.at(i)->GetOffsetInNano() > maxOffset) {
			maxOffset = periodicRunnables.at(i)->GetOffsetInNano();
		}
	}
	unsigned long int hyperPeriod = std::accumulate(periods.begin(),
			periods.end(), 1, lcm);
	return hyperPeriod + maxOffset;
}

}
