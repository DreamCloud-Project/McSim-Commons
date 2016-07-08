#ifndef MYGRAPH_H_
#define MYGRAPH_H_

#include "AmApplication.h"
#include "AmTask.h"
#include "dcRunnableCall.h"
#include "dcRunnableEdge.h"
#include "dcTask.h"
#include "dcTaskEdge.h"
#include "dcTaskGraph.h"

namespace DCApplication {

class dcApplication {

public:

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
	static void dumpTaskGraphFile(string file, vector<dcTask *> tasks);
	static void dumpTaskAndRunnableGraphFile(string file, vector<dcTask *> tasks);

	// label
	vector<dcLabel*> GetAllLabels(AmApplication* AmApplicationIn);

	// cores
	vector<dcCore*> GetCores(AmApplication* AmApplicationIn);

	// create graph entities
	void CreateGraphEntities(dcTaskGraph*, AmApplication*, bool seqDep);

	// Activation events or stimulus in amalthea terminology
	vector<dcActEvent*> GetPeriodicEvents(AmApplication* AmApplicationIn);
	vector<dcActEvent*> GetSporadicEvents(AmApplication* AmApplicationIn);
	vector<dcActEvent*> GetAperiodicEvent(AmApplication* AmApplicationIn);
	vector<dcActEvent*> GetDeterministicIrregular(AmApplication* AmApplicationIn);
	vector<dcActEvent*> GetInterprocessEvent(AmApplication* AmApplicationIn);
};
}

#endif /* MYGRAPH_H_ */
