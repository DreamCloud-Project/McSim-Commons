/*
 * MicroWorkloadGenerator.hxx
 *
 *  Created on: Jul 1, 2016
 *      Author: manu
 */

#ifndef SRC_MICROWORKLOADGENERATOR_HXX_
#define SRC_MICROWORKLOADGENERATOR_HXX_

#include <vector>
#include "../parser/AmApplication.h"
#include "../parser/dcApplication.h"

using namespace DCApplication;

class dcMwGenerator {

public:

	std::vector<dcTaskGraph*> MwGenerate(dcTaskGraph* dcTaskGraphIn,
			AmApplication* AmApplicationIn, size_t width, size_t depth,
			size_t num);

private:

	// Methods
	dcTaskGraph* MwCreateGraph();
	void MwAddTasks(dcTaskGraph* dcTaskGraphIn, dcTask* TaskIn);
	vector<int> MwSelectionSort(vector<int> input);
	vector<vector<int> > MwPartition(vector<int> input, int num);
	vector<vector<int>> MwPartition(vector<int> vectorIn);
	vector<vector<string>> MwPartitionResource(vector<string> In);
	vector<pair<dcTask*, pair<string, int>>> MwAnalyseTask(dcTask* TaskIn);
	vector<dcTask*> MwGetSequence(dcTask* TasksIn);
	dcTask* MwClone(dcTask* TaskIn);
	bool MwExisting(size_t size, vector<vector<dcTask*>> PatternIn);//CHeck if pattern exist with equal number of tasks. If it does, do not add
	bool MwIsExisting(vector<dcTask*>, dcTask*);
	void MwAssignAttributes(AmApplication*, vector<vector<dcTask*>>);
	vector<dcTaskGraph*> MwCreateMicroIndependent(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> MwCreateMicroFork(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> MwCreateMicroJoin(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> MwCreateMicroSequential(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> MwCreateMicroFeedback(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> MwCreateMicroFurther(vector<vector<dcTask*>>);
	vector<dcTaskGraph*> MwCreateMicroCustom(vector<vector<dcTask*>>);
	bool MwIsIn(vector<int>, int);
	vector<dcTask*> MwGetAllTasks(dcTaskGraph* dcTaskGraphIn);

	// fields
	vector<pair<dcTask*, pair<string, int>>> MwResult;
	vector<dcTask*> MwResult2;
	pair<vector<dcTask*>, bool> MwResult3;
	dcTask* MwFirstTask; //needed to distinguish between sequence and feeback tasks
	unsigned int counter;

};

#endif /* SRC_MICROWORKLOADGENERATOR_HXX_ */
