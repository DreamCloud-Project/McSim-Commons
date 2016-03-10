/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#ifndef MODULES_DREAMCLOUD_PLATFORM_SCLIB_MAPPING_HEURISTIC_WEKA_HXX_
#define MODULES_DREAMCLOUD_PLATFORM_SCLIB_MAPPING_HEURISTIC_WEKA_HXX_

#include "dcMappingHeuristicI.hxx"
#include <vector>
#include <random>

namespace dreamcloud {
namespace platform_sclib {

class dcMappingHeuristicWeka: public dcMappingHeuristicI {

public:
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
	dcMappingHeuristicWeka(int seed);

	void setFiles(string staticFile, string randomFile);

private:
	int xLabel;
	int yLabel;
	int NoCXSize;
	int NoCYSize;
	std::mt19937 gen;

	struct RunMappings {
		int i = 0;
		string name;
		vector<dcMappingLocation> locs;
	};

	map<string, RunMappings> staticMappings;
	map<string, RunMappings> randomMappings;
	vector<string> randomRuns;
	vector<string> staticRuns;
};

}
}
#endif
