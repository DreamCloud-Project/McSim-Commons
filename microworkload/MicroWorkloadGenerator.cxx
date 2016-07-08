#include "MicroWorkloadGenerator.hxx"
#include <queue>

dcTaskGraph * dcMwGenerator::MwCreateGraph() {
	dcTaskGraph* TaskGraphIn = new dcTaskGraph();
	return TaskGraphIn;
}

bool dcMwGenerator::MwIsIn(vector<int> Vals, int Val) {
	bool In = false;
	for (size_t i = 0; i < Vals.size(); i++) {
		if (Vals.at(i) == Val) {
			In = true; // is int
			break;
		}
	}
	return In;
}

void dcMwGenerator::MwAddTasks(dcTaskGraph* TaskGraphIn, dcTask* TaskIn) {
	if (TaskGraphIn->GetHead() == NULL && TaskGraphIn->GetTail() == NULL) {
		TaskGraphIn->SetHead(TaskIn);
		TaskGraphIn->SetTail(TaskIn);
	} else {
		TaskGraphIn->GetTail()->SetNext(TaskIn);
		TaskGraphIn->SetTail(TaskIn);
	}
}
vector<dcTask*> dcMwGenerator::MwGetSequence(dcTask* MyTask) {
	static queue<dcTask*> TaskQueue;
	vector<dcTask*> Children;

	if (MyTask->GetVisited() == 0) {
		dcTask* Temp = MyTask;
		MwResult2.push_back(Temp);
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
			if (Children.front() != MwFirstTask) {
				TaskQueue.push(Children.front());
			}
		}
	}
	while (!TaskQueue.empty()) {
		dcTask* MyTask = TaskQueue.front();
		TaskQueue.pop();
		MwGetSequence(MyTask);
	}

	return MwResult2;
}

dcTask* dcMwGenerator::MwClone(dcTask* TaskIn) {
	dcTask* Task = new dcTask(TaskIn->GetName() + std::to_string(counter));
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
	Task->SetID(TaskIn->GetID() + std::to_string(counter));
	Task->SetTaskID(counter);
	Task->SetEdges(NULL);
	Task->SetNext(NULL);
	Task->SetRunnables(TaskIn->GetRunnableCalls());
	counter++;
	return Task;
}

bool dcMwGenerator::MwExisting(size_t size, vector<vector<dcTask*>> PatternIn) {
	bool Exist = 0;
	for (size_t i = 0; i < PatternIn.size(); i++) {
		if (size == PatternIn.at(i).size()) {
			Exist = 1;
			break;
		}
	}

	return Exist;
}

bool dcMwGenerator::MwIsExisting(vector<dcTask*> TasksIn, dcTask* TaskIn) {
	bool Existing = 0;
	for (size_t i = 0; i < TasksIn.size(); i++) {
		if (TaskIn->GetID() == TasksIn.at(i)->GetID()) {
			Existing = 1;
			break;
		}
	}

	return Existing;
}

vector<vector<int>> dcMwGenerator::MwPartition(vector<int> vectorIn) {
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
	left = MwPartition(l);
	right = MwPartition(r);

	for (size_t i = 0; i < left.size(); i++)
		result.push_back(left[i]);
	for (size_t i = 0; i < right.size(); i++)
		result.push_back(right[i]);
	return result;
}

vector<pair<dcTask*, pair<string, int>>> dcMwGenerator::MwAnalyseTask(dcTask* MyTask) {
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
			MwResult.push_back(Temp);
		}

	}

	while(!TaskQueue.empty()) {
		dcTask* MyTask = TaskQueue.front();
		TaskQueue.pop();
		MwAnalyseTask(MyTask);
	}

	//do some refinement
	string temp = "J";
	dcTask* JoinTask = NULL;
	for(size_t i = 0; i < MwResult.size(); i++) {
		if(MwResult.at(i).second.first == temp.c_str()) {
			JoinTask = MwResult.at(i).first;
		}

		unsigned int j = 0;
		while(j < MwResult.size()) {
			dcTaskEdge* Edges = MwResult.at(j).first->GetEdges();
			while(Edges != NULL) {
				if(Edges->GetType() == 1) {
					if(Edges->GetConnectTo() == JoinTask) {
						MwResult.erase(MwResult.begin()+j);
					}
				}
				Edges = Edges->GetNext();
			}
			j++;
		}
	}
	return MwResult;
}

void dcMwGenerator::MwAssignAttributes(AmApplication* dcApplicationIn,
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
			//srand((unsigned int) time(NULL));
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
						bool In = MwIsIn(Visited, Position);
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
						bool In = MwIsIn(Visited, Position);
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
				vector<vector<int>> PartitionedVal = MwPartition(
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
				vector<vector<int>> PartitionedVal = MwPartition(
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

vector<dcTaskGraph*> dcMwGenerator::MwCreateMicroIndependent(
		vector<vector<dcTask*>> Independents) {
	vector<dcTaskGraph*> MicroIndependent;
	size_t count = 0;
	while (count < Independents.size()) {
		dcTaskGraph* Graph = MwCreateGraph();
		for (size_t i = 0; i < Independents.at(count).size(); i++) {
			dcTask* Temp = Independents.at(count).at(i);
			Temp->SetNext(0);
			MwAddTasks(Graph, Temp);
		}
		MicroIndependent.push_back(Graph);
		count++;
	}
	return MicroIndependent;
}
vector<dcTaskGraph*> dcMwGenerator::MwCreateMicroFork(
		vector<vector<dcTask*>> Fork) {
	vector<dcTaskGraph*> MicroFork;
	//fork
	size_t count = 0;
	while (count < Fork.size()) {
		dcTaskGraph* Graph = MwCreateGraph();
		for (size_t i = 0; i < Fork.at(count).size(); i++) {
			dcTask* Temp = Fork.at(count).at(i);
			Temp->SetNext(0);
			MwAddTasks(Graph, Temp);
		}
		MicroFork.push_back(Graph);
		count++;
	}
	return MicroFork;
}
vector<dcTaskGraph*> dcMwGenerator::MwCreateMicroJoin(
		vector<vector<dcTask*>> Join) {
	vector<dcTaskGraph*> MicroJoin;
	size_t count = 0;

	//join
	while (count < Join.size()) {
		dcTaskGraph* Graph = MwCreateGraph();
		for (int i = (Join.at(count).size() - 1); i >= 0; i--) {
			dcTask* Temp = Join.at(count).at(i);
			Temp->SetNext(0);
			MwAddTasks(Graph, Temp);
		}
		MicroJoin.push_back(Graph);
		count++;
	}
	return MicroJoin;
}
vector<dcTaskGraph*> dcMwGenerator::MwCreateMicroSequential(
		vector<vector<dcTask*>> Sequential) {
	vector<dcTaskGraph*> MicroSequential;
	size_t count = 0;

	while (count < Sequential.size()) {
		dcTaskGraph* Graph = MwCreateGraph();
		for (size_t i = 0; i < Sequential.at(count).size(); i++) {
			dcTask* Task = MwClone(Sequential.at(count).at(i));
			dcTask* Dest = NULL;
			bool valid = 0;

			if (i != (Sequential.at(count).size() - 1)) {
				int j = i + 1;
				Dest = MwClone(Sequential.at(count).at(j));
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
			MwAddTasks(Graph, Task);
		}
		MicroSequential.push_back(Graph);
		count++;
	}
	return MicroSequential;
}
vector<dcTaskGraph*> dcMwGenerator::MwCreateMicroFeedback(
		vector<vector<dcTask*>> Feedback) {
	vector<dcTaskGraph*> MicroFeedback;
	size_t count = 0;
	while (count < Feedback.size()) {
		dcTaskGraph* Graph = MwCreateGraph();
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
			MwAddTasks(Graph, Task);
		}
		MicroFeedback.push_back(Graph);
		count++;
	}
	return MicroFeedback;
}
vector<dcTaskGraph*> dcMwGenerator::MwCreateMicroFurther(
		vector<vector<dcTask*>> Further) {
	//graphs for further micro-workloads
	vector<dcTaskGraph*> MicroFurther;
	size_t count = 0;
	while (count < Further.size()) {
		dcTaskGraph* Graph = MwCreateGraph();
		for (size_t i = 0; i < Further.at(count).size(); i++) {
			dcTask* Temp = Further.at(count).at(i);
			Temp->SetNext(0);
			MwAddTasks(Graph, Temp);
		}
		MicroFurther.push_back(Graph);
		count++;
	}
	return MicroFurther;
}
vector<dcTaskGraph*> dcMwGenerator::MwCreateMicroCustom(
		vector<vector<dcTask*>> Custom) {
	//graphs for further micro-workloads
	vector<dcTaskGraph*> MicroCustom;
	size_t count = 0;
	while (count < Custom.size()) {
		dcTaskGraph* Graph = MwCreateGraph();
		for (size_t i = 0; i < Custom.at(count).size(); i++) {
			dcTask* Temp = Custom.at(count).at(i);
			Temp->SetNext(0);
			MwAddTasks(Graph, Temp);
		}
		MicroCustom.push_back(Graph);
		count++;
	}
	return MicroCustom;
}
vector<int> dcMwGenerator::MwSelectionSort(vector<int> input) {
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

vector<dcTask*> dcMwGenerator::MwGetAllTasks(dcTaskGraph* dcTaskGraphIn) {
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

vector<dcTaskGraph*> dcMwGenerator::MwGenerate(dcTaskGraph *TaskGraphIn,
		AmApplication* dcApplicationIn, size_t width, size_t depth,
		size_t num) {

	// init random number generator with a fixed seed for reproductibility concerns
	srand(42);

	//basic patterns
	vector<dcTask*> Independent;
	vector<vector<dcTask*>> Fork;
	vector<vector<dcTask*>> Join;
	vector<vector<dcTask*>> Feedback;
	vector<vector<dcTask*>> Sequential;
	vector<dcTask*> SequenceTemp;
	vector<vector<dcTask*>> Further;
	vector<vector<dcTask*>> Independents;
	unsigned int NumOfTasks = MwGetAllTasks(TaskGraphIn).size(); //microworkload should be less than the size of the input application

	dcTask *MyTask = TaskGraphIn->GetHead();
	if (MyTask != NULL) {
		size_t count = 0;
		size_t count2 = 0; //counter for join tasks
		while (MyTask != NULL) {
			count = 0;
			count2 = 0;
			dcTaskEdge* Edge = MyTask->GetEdges();
			if (Edge == NULL) { //independent tasks
				dcTask* Temp = MwClone(MyTask);
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
				dcTask* Task = MwClone(MyTask); //dest
				dcTaskEdge* EdgeT = MyTask->GetEdges(); //get back edges
				Task->SetEdges(0);
				vector<dcTask*> Tasks;
				Tasks.push_back(Task);
				while (EdgeT != NULL) {
					if (EdgeT->GetType() == 0) {
						dcTaskEdge *E = new dcTaskEdge();
						E->SetType(1);
						E->SetNext(0);
						dcTask* Temp = MwClone(EdgeT->GetConnectTo()); //src task
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
				bool Exist = MwExisting(Tasks.size(), Join);
				if (Exist != 1) {
					Join.push_back(Tasks);
				}
			}

			if (count > 1) { //fork structure
				dcTask* Task = MwClone(MyTask);
				dcTaskEdge* EdgeT = MyTask->GetEdges();
				Task->SetEdges(0);
				vector<dcTask*> Tasks;

				Tasks.push_back(Task);
				while (EdgeT != NULL) {

					if (EdgeT->GetType() == 1) {
						dcTaskEdge *E = new dcTaskEdge();
						E->SetType(1);
						E->SetNext(0);
						dcTask* Temp = MwClone(EdgeT->GetConnectTo());
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
				bool Exist = MwExisting(Tasks.size(), Fork);
				if (Exist != 1) {
					Fork.push_back(Tasks);
				}

			}

			if (count == 1) { //sequence or feedback task
				MwFirstTask = MyTask;
				SequenceTemp = MwGetSequence(MyTask);
				if (SequenceTemp.size() > 1) {
					MwResult2.clear();
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
						bool Exist = MwExisting(SequenceTemp.size(), Feedback);
						if (Exist != 1) {
							Feedback.push_back(SequenceTemp);
						}

					} else {
						bool Exist = MwExisting(SequenceTemp.size(),
								Sequential);
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
			MwResult = MwAnalyseTask(MyTask);
			vector<dcTask*> ForkTemp;
			vector<dcTask*> SeqTemp;

			if (MwResult.size() > 1 && !MwResult.empty()) {
				for (size_t i = 0; i < MwResult.size(); i++) {
					dcTask* Task = MwResult.at(i).first;
					string Type = MwResult.at(i).second.first;
					int Degree = MwResult.at(i).second.second;

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

							FirstType = MwResult.at(i).second.first;

							if (Type == "F" && ForkDegree == 0) {
								ForkDegree = MwResult.at(i).second.second;
							}
						} else {
							if (FirstType == Type) { //keep the chain
								Temp.push_back(Temp2);
							} else { // I have a further workload
								Temp.push_back(Temp2);

								if (ForkDegree == 0) {
									FirstType = MwResult.at(i).second.first;
									vector<dcTask*> Temp3;
									for (size_t j = 0; j < Temp.size(); j++) {
										if (j == 0) {
											dcTask* Original = Temp.at(j).first;
											dcTask* Cloned = MwClone(
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

														dcTask* Dest = MwClone(
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
											dcTask* Cloned = MwClone(
													Temp.at(j).first);
											string WhatType =
													Temp.at(j).second.first;

											//check whether Task is in the buffer
											bool Exist = MwIsExisting(Temp3,
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
																		MwClone(
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
																	MwClone(
																			Edge->GetConnectTo());
															//Check if it's existing already
															bool Exist =
																	MwIsExisting(
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
																	MwClone(
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
									ForkDegree = MwResult.at(i).second.second;
								}
							}
						}
					}
				}
			}

			MwResult.clear();
			MyTask = MyTask->GetNext();

		}
	}

	vector<vector<dcTask*>> CustomWorkloads;

	for (size_t i = 0; i < num; i++) {
		//user oriented workloads
		//srand((unsigned int) time(NULL));
		vector<dcTask*> Tasks = MwGetAllTasks(TaskGraphIn);
		vector<dcTask*> Custom;

		dcTask *FirstTask = NULL;
		dcTask* MyForkTask = NULL;
		bool flag = false;
		vector<dcTask*> Seq;

		size_t count = 0;
		while (count < depth) {

			if (count == 0) { //sequential tasks
				size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
				FirstTask = MwClone(Tasks.at(index));
				Custom.push_back(FirstTask);

			} else if (count == 1) { // fork
				size_t index = rand() % (Tasks.size() - 1 + 1) + 0;
				MyForkTask = MwClone(Tasks.at(index));
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
					dcTask* MyForkChildren = MwClone(Tasks.at(index2));
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
					dcTask* MyTask = MwClone(Tasks.at(index));
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
					dcTask* MyTask = MwClone(Tasks.at(index));
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
						dcTask* MyTask = MwClone(Tasks.at(index));
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
						dcTask* MyTask = MwClone(Tasks.at(index));
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
				dcTask* IndTask = MwClone(Tasks.at(index));
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
				Independents.at(0).push_back(MwClone(Independent.at(index)));
			}

		} else {

			for (size_t i = 0; i < Independent.size(); i++) {
				size_t index = rand() % (Independent.size() - 1 + 1) + 0;
				Independents.at(0).push_back(MwClone(Independent.at(index)));
			}
		}
	}

	//assign attributes to workloads
	MwAssignAttributes(dcApplicationIn, Independents);
	MwAssignAttributes(dcApplicationIn, Further);
	MwAssignAttributes(dcApplicationIn, Fork);
	MwAssignAttributes(dcApplicationIn, Join);
	MwAssignAttributes(dcApplicationIn, Sequential);
	MwAssignAttributes(dcApplicationIn, Feedback);
	MwAssignAttributes(dcApplicationIn, CustomWorkloads);

	//CREATE MICROWORKLOADS

	vector<dcTaskGraph*> MicroIndependent = MwCreateMicroIndependent(
			Independents);
	vector<dcTaskGraph*> MicroFork = MwCreateMicroFork(Fork);
	vector<dcTaskGraph*> MicroJoin = MwCreateMicroJoin(Join);
	vector<dcTaskGraph*> MicroSequential = MwCreateMicroSequential(Sequential);
	vector<dcTaskGraph*> MicroFeedback = MwCreateMicroFeedback(Feedback);
	vector<dcTaskGraph*> MicroFurther = MwCreateMicroFurther(Further);
	vector<dcTaskGraph*> MicroCustom = MwCreateMicroCustom(CustomWorkloads);

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

	return WorkLoads;
}

vector<vector<int> > dcMwGenerator::MwPartition(vector<int> input, int num) {
	vector<vector<int> > result;
	vector<int> temp = MwSelectionSort(input);
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
