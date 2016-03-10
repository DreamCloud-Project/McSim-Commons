/*
 * dcMappingHeuristic.hxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#ifndef MODULES_DREAMCLOUD_PLATFORM_SCLIB_DCMAPPINGHEURISTIC_HXX_
#define MODULES_DREAMCLOUD_PLATFORM_SCLIB_DCMAPPINGHEURISTIC_HXX_

#include <utility>
#include <map>

using namespace std;

namespace dreamcloud {
namespace platform_sclib {

class dcMappingHeuristicI {

public:
	virtual ~dcMappingHeuristicI() {
	}
	;

	// Mapping functions
	typedef std::pair<int, int> dcMappingLocation;
	virtual dcMappingLocation mapRunnable(unsigned long int time,
			const string& runnableId, const string& runnableName,
			const string& taskName, const string& taskId, unsigned int idInTask,
			unsigned int periodId) = 0;
	virtual void runnableMapped(unsigned long int time,
			const string& runnableId) = 0;
	virtual dcMappingLocation mapLabel(const string& labelId,
			unsigned long int time, const string& labelName) = 0;
	virtual void switchMode(unsigned long int time, const string& newModeFile,
			const string& newModeName) = 0;

	// Hardware model construction functions
	virtual void addCoreType(const string& coreType, double energyMultiplier,
			double switchingPModeTimePenalty,
			double switchingPModeEnergyPenalty, int maxFrequencyInHz,
			map<double, double> voltageToFreq, int voltToFreqLevel) = 0;
	virtual void addProcessingCore(const string& name, int x, int y,
			const string& coreType, unsigned int clockRatio,
			unsigned int quartz, unsigned int ticksPerCycle,
			double staticEnergyValue, const string& staticEnergyUnit,
			double energyMultiplier, int pMode) = 0;
	virtual void setNocSize(unsigned int NoCXSize, unsigned int NoCYSize) = 0;

	// Application model functions
	virtual void setApp(const string& appFile) = 0;

	// Platform feedbacks
	virtual void completeRunnable(unsigned long int time,
			const string& runnableId) = 0;
};

}
}

#endif /* MODULES_DREAMCLOUD_PLATFORM_SCLIB_DCMAPPINGHEURISTIC_HXX_ */
