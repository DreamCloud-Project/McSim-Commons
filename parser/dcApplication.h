#ifndef MYGRAPH_H_
#define MYGRAPH_H_

#include "dcRunnableCall.h"
#include "dcTask.h"
#include "dcTaskEdge.h"
#include "dcTaskGraph.h"
#include "AmTask.h"
#include "AmApplication.h"
#include "dcRunnableEdge.h"

namespace DCApplication {

class dcApplication {

public:

	dcApplication();
	~dcApplication();

	// Task
	dcTaskGraph* createGraph(string Name);
	void AddTasks(dcTaskGraph* dcTaskGraphIn, AmApplication* AmApplicationIn);
	void PrintTasks(dcTaskGraph* dcTaskGraphIn);
	vector<dcTask*> GetAllTasks(dcTaskGraph* dcTaskGraphIn);
	void AdddcTaskEdges(dcTaskGraph* dcTaskGraphIn,
			AmApplication* AmApplicationIn);
	vector<dcTask*> GetdcTaskEdges(dcTaskGraph* dcTaskGraphIn, dcTask* TaskIn);

	// Runnable
	void AddRunnables(dcTaskGraph* dcTaskGraphIn,
			AmApplication* AmApplicationIn);
	vector<dcRunnableCall*> GetTaskRunnableCalls(dcTaskGraph* dcTaskGraphIn,
			dcTask* TaskIn);
	vector<int> GetAllPriorities(dcTaskGraph* dcTaskGraphIn);
	vector<dcRunnableCall*> GetRunnableCall(dcTask* task, string Id);
	vector<dcRunnableCall*> GetAllRunnables(dcTaskGraph* dcTaskGraphIn);
	vector<dcInstruction*> GetAllInstructions(dcTaskGraph* dcTaskGraphIn);
	void AdddcRunnableEdges(dcTaskGraph* dcTaskGraphIn, bool seqDep);
	vector<dcRunnableCall*> GetdcRunnableEdges(dcTaskGraph* dcTaskGraphIn,
			dcTask* TaskIn, dcRunnableCall* RunnableIn);
	vector<pair<string, pair<int, int>>> GetAllRunnableComputation(dcTaskGraph* dcTaskGraphIn);
	vector<dcRunnableCall*> GetIndependentNonPeriodicRunnables(dcTaskGraph* dcTaskGraphIn);
	vector<dcRunnableCall*> GetIndependentRunnables(dcTaskGraph* dcTaskGraphIn);
	vector<dcRunnableCall*> GetPeriodicAndSporadicRunnables(dcTaskGraph* dcTaskGraphIn, bool seqDep);
	unsigned long int GetHyperPeriodWithOffset(dcTaskGraph* graph);
	void dumpLabelAccesses(dcTaskGraph* dcTaskGraphIn, string outputFolder);
	void dumpLabelAccessesPerRunnable(dcTaskGraph* dcTaskGraphIn, string outputFolder);
	void dumpRunnablesToFiles(dcTaskGraph* dcTaskGraphIn, string outputFolder); // Added 02_09_2015
	void dumpTaskGraphFile(string file, vector<dcTask *> tasks);
	void dumpTaskAndRunnableGraphFile(string file, vector<dcTask *> tasks);

	// label
	vector<dcLabel*> GetAllLabels(AmApplication* AmApplicationIn);

	// cores
	vector<dcCore*> GetCores(AmApplication* AmApplicationIn);

	// create graph entities
	void CreateGraphEntities(dcTaskGraph*, AmApplication*, bool seqDep);

	// graph analysis
	void PrintGraph(dcTaskGraph* dcTaskGraphIn, ofstream& myfile);//print task and runnable graph

	// Microworkloads
	dcTaskGraph* CreateGraph();
	void AddTasks(dcTaskGraph* dcTaskGraphIn, dcTask* TaskIn);

	vector<int> SelectionSort(vector<int> input);
	vector<vector<int> > Partition(vector<int> input, int num);

	// variables
	vector<pair<dcTask*, pair<string, int>>> Result;
	vector<dcTask*> Result2;
	pair<vector<dcTask*>, bool> Result3;
	dcTask* FirstTask;//needed to distinguish between sequence and feeback tasks
	vector<vector<int>> Partition(vector<int> vectorIn);
	vector<vector<string>> PartitionResource(vector<string> In);
	bool IsVisted(vector<string>, string);
	vector<pair<dcTask*, pair<string, int>>> AnalyseTask(dcTask* TaskIn);
	bool Visited;

	void AnalyzeGraph(dcTaskGraph* dcTaskGraphIn);
	void AnalyzeGraph_(dcTaskGraph* dcTaskGraphIn);
	vector<dcTask*> GetSequence(dcTask* TasksIn);
	dcTask* Clone(dcTask* TaskIn);
	void DeleteEdge(dcTask*, bool);
	vector<dcTaskGraph*> GenerateMicroworkloads(dcTaskGraph* dcTaskGraphIn, AmApplication* AmApplicationIn, size_t width, size_t depth, size_t num);
	bool Existing(size_t size, vector<vector<dcTask*>> PatternIn);//CHeck if pattern exist with equal number of tasks. If it does, do not add
	bool IsExisting(vector<dcTask*>, dcTask*);
	pair<vector<dcTask*>, bool> TranverseGraph(dcTask* TaskIn);
	void AssignAttributes(AmApplication*, vector<vector<dcTask*>>);
	vector<dcTaskGraph*> CreateMicroIndependent(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> CreateMicroFork(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> CreateMicroJoin(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> CreateMicroSequential(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> CreateMicroFeedback(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> CreateMicroFurther(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> CreateMicroCustom(vector<vector<dcTask*>>);

	// Activation events or stimulus in amalthea terminology
	vector<dcActEvent*> GetPeriodicEvents(AmApplication* AmApplicationIn);
	vector<dcActEvent*> GetSporadicEvents(AmApplication* AmApplicationIn);
	vector<dcActEvent*> GetAperiodicEvent(AmApplication* AmApplicationIn);
	vector<dcActEvent*> GetDeterministicIrregular(AmApplication* AmApplicationIn);
	vector<dcActEvent*> GetInterprocessEvent(AmApplication* AmApplicationIn);

	//Preemptive
	bool IsIn(vector<int>, int);

private:
	int Size;
	int EdgeSize;
	dcTaskEdge* Head;
};
}

#endif /* MYGRAPH_H_ */
