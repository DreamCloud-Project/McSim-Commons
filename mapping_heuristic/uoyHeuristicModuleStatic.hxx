#ifndef IA_HEURISTICMODULESTATIC_H
#define IA_HEURISTICMODULESTATIC_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <regex>
#include <boost/lexical_cast.hpp>
#include "dcMappingHeuristicI.hxx"

namespace dreamcloud {
namespace platform_sclib {

using namespace std;

class uoyHeuristicModuleStatic: public dcMappingHeuristicI {

public:

	void ParseModeListFile(const string& strDirectory);
	void ParseModeChangeTiming(const string& strDirectory);

	//////////////////////////////////////////////
	//////////////// Interface  //////////////////
	//////////////////////////////////////////////

	// Mapping functions and type definitions
	dcMappingLocation mapRunnable(unsigned long int time,
			const string& runnableId, const string& runnableName,
			const string& taskName, const string& taskId, unsigned int idInTask,
			unsigned int periodId);
	void runnableMapped(unsigned long int time, const string& runnableId);
	dcMappingLocation mapLabel(const string& labelId, unsigned long int time,
			const string& labelName);
	void switchMode(unsigned long int time, const string& newModeFile,
			const string& newModeName);

	// Hardware model functions
	void addCoreType(const string& coreType, double energyMultiplier,
			double switchingPModeTimePenalty,
			double switchingPModeEnergyPenalty, int maxFrequencyInHz,
			map<double, double> voltageToFreq, int voltToFreqLevel);
	void addProcessingCore(const string& name, int x, int y,
			const string& coreType, unsigned int clockRatio,
			unsigned int quartz, unsigned int ticksPerCycle,
			double staticEnergyValue, const string& staticEnergyUnit,
			double energyMultiplier, int pMode);
	void setNocSize(unsigned int NoCXSize, unsigned int NoCYSize);

	// Application model functions
	void setApp(const string& appFile);

	// Platform feedbacks
	void completeRunnable(unsigned long int time, const string& runnableId);
	uoyHeuristicModuleStatic();

private:
	vector<string> Modes_;
	map<pair<string, string>, pair<unsigned long int, string> > ModeTransitionTimeFromSourceToDest_;
	map<pair<string, string>, int> MapForRunnableAndLabelInMode_;
	string CurrentMode_;
	string FutureMode_;
	unsigned long int TimeOfModeChangeRequest_;

	string RemoveExtension(const string& FileName);
	void SwitchModeIfRequired(unsigned long int CurrentTime);
	void ParseModeFile(const string& strDirectory, const string& strName);
	void AddMapForRunnableInModeItem(const string& Mode,
			const string& RunnableName, const int TargetCoreIndex);
	void AddModeTransitionTimeFromSourceToDestItem(
			const string& PreviousModeName, const string& NextModeName,
			const unsigned long int TransitionTime,
			const string& TransitionUnit);
	unsigned long int GetModeTransitionTimeFromSourceToDest(
			const string& PreviousModeName, const string& NextModeName);
	int GetMapForRunnableInModeItem(const string& Mode,
			const string& RunnableName);
	void AddMode(const string& ModeName);
	string GetCurrentMode();
	void ClearModes();
	void SetCurrentMode(const string& Mode);
	void ClearFutureMode();
	string GetFutureMode();
	void PrintModes();
	void SetFutureMode(const string& Mode);
	void SetTimeOfModeChangeRequest(unsigned long int TimeOfModeChangeRequest);
	unsigned long int GetTimeOfModeChangeRequest();
	unsigned long int GetSwitchingTime();
	dcMappingLocation indexToLocation(int index);
};
}
}

#endif // IA_HEURISTICMODULESTATIC_H
