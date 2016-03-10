#ifndef AM_RUNNABLE_CLASS_H
#define AM_RUNNABLE_CLASS_H

#include <iostream>
#include <string>
#include <vector>

#include "dcInstruction.h"
#include "dcRunnableExecutionTime.h"
#include "dcTag.h"


namespace DCApplication
{
   using namespace std;

   /**
    * This class represent the definition of a runnable class
    * as specified in Amalthea with the <runnables> </runnables> xml tag.
    */
	class AmRunnableClass {
		string Name;
		dcTag* Tags;
		string ID;
		vector<dcInstruction*> Instructions;
		int Size;
		vector<string> ReleasedTasks;
		bool OrderedTaskReleased;
		dcRunnableExecutionTime* ExecutionTime;
		string RunSource;
		vector<string> RunDestination;
		int counter;

	public:
		AmRunnableClass(string NameIn);
		~AmRunnableClass();
		void SetName(string NameIn);
		string GetName();
		void SetTags(dcTag* TagsIn);
		dcTag* GetTags();
		int GetSize();
		void SetSize(int SizeIn);
		void AddInstruction(dcInstruction *InstructionIn);
		void Print();
		void SetID(string IDIn);
		string GetID();
		double GetWCETInTick();
		bool CheckIfIncludesExecutionCyclesDeviationInstruction();
		string GetTheFirstDeviationType();
		double GetTheFirstDeviationRemainPromille();
		double GetTheFirstDeviationStandardDeviation();
		double GetTheFirstDeviationLowerBound();
		double GetTheFirstDeviationUpperBound();
		double GetTheFirstDeviationMean();
		string GetRunSource();
		void SetRunSource(string RunSourceIn);
		vector<string> GetRunnableDestination();


		vector<dcInstruction*> GetAllInstructions();
		vector<string> GetReleasedTasks();
		void AddReleasedTask(string releasedTaskIn);

		void SetOrderedTaskReleased(bool OrderedTaskReleasedIn);
		bool GetOrderedTaskReleased();

		void SetRunnableExecutionTime(dcRunnableExecutionTime* MyExecutionTimeIn);
		dcRunnableExecutionTime* GetRunnableExecutionTime();

		vector<string>GetRunDestination();
		void AddRunDestination(string RunDestinationIn);
		
		int GetCounter();
		void SetCounter(int RunnableIDIn);
	}; 


}

#endif //
