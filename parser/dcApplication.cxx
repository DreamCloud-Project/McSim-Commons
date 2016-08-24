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
					if (curDctask->GetTaskRunnableCalls() == 0) {
						curDctask->SetRunnables(dcRunCall);
						curDctask->GetTaskRunnableCalls()->SetTail(dcRunCall);
					} else {
						curDctask->GetTaskRunnableCalls()->GetTail()->SetNext(
								dcRunCall);
						curDctask->GetTaskRunnableCalls()->SetTail(dcRunCall);
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
			dcRunnableCall* runCall = MyTask->GetTaskRunnableCalls();
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
	while (task != NULL) {
		dcRunnableCall* runCall = task->GetTaskRunnableCalls();
		while (runCall != NULL) {
			if (runCall->GetRunClassId() == runClassId) {
				result.push_back(runCall);
			}
			runCall = runCall->GetNext();
		}
		task = task->GetNext();
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
			dcRunnableCall* MyRunnables = MyTask->GetTaskRunnableCalls();
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
		dcRunnableCall* runnable = task->GetTaskRunnableCalls();
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
	dcTask* headTask = task;
	if (task == 0) {
		cout << "No Tasks in the dcApplication, cannot add runnables" << endl;
	} else {
		while (task != NULL) {

			dcRunnableCall* run = task->GetTaskRunnableCalls();
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

					// Assert there is only a single runnable call with this run class name
					// in the current task
					string destRunClassName =
							run->GetRunClassNameDestinations().at(i);
					vector<dcRunnableCall*> destRunCalls = GetRunnableCall(headTask,
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
			dcRunnableCall* MyRunnables = MyTask->GetTaskRunnableCalls();
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

void dcApplication::CreateGraphEntities(dcTaskGraph* dcTaskGraphIn,
		AmApplication* AmApplication, bool seqDep) {
	AddTasks(dcTaskGraphIn, AmApplication);
	AdddcTaskEdges(dcTaskGraphIn, AmApplication);
	AddRunnables(dcTaskGraphIn, AmApplication);
	AdddcRunnableEdges(dcTaskGraphIn, seqDep);
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
		} else {
			color = "black";
			period << "Non periodic";
		}
		dcGraphDotFile << "\t\tcolor=" << color << ";" << endl;
		dcGraphDotFile << "\t\tlabel=<<font color=\"" << color << "\">"
				<< (*it)->GetName() << period.str() << "</font>>;" << endl;
		dcRunnableCall *run = (*it)->GetTaskRunnableCalls();
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
			dcRunnableCall* MyRunnables = MyTask->GetTaskRunnableCalls();
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
			dcRunnableCall* MyRunnables = MyTask->GetTaskRunnableCalls();
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
			dcRunnableCall* runnable = task->GetTaskRunnableCalls();
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
