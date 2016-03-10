/*
 * dcMappingHeuristicZigZagSM.cxx
 *
 *  Created on: 15 Oct 2015
 *      Author: effiong
 */

#include "dcMappingHeuristicZigZagSM.hxx"

namespace dreamcloud {
namespace platform_sclib {

/**
 * Map the runnables in zigzag fashion on all cores except Core(0,0) since this is use as memory.
 *
 *  TAKE CARE: this mapper is dependent from the runnables order
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicZigZagSM::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {

	dcMappingLocation loc = dcMappingLocation(xRunnable, yRunnable);
	yRunnable++;
	if (yRunnable == NoCYSize) {
		yRunnable = 0;
		xRunnable++;
		if (xRunnable == NoCXSize) {
			xRunnable = 0;
		}
	}
	return loc;

	return loc;
}

/**
 * Map label on core (0,0).
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicZigZagSM::mapLabel(
		const string& labelId, unsigned long int time,
		const string& labelName) {
	pair<int, int> Location = make_pair(0, 0);
	dcMappingLocation loc = dcMappingLocation(Location.first, Location.second);
	return loc;
}

/**
 * Save the NoC size for labels and runnables mapping.
 */
void dcMappingHeuristicZigZagSM::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicZigZagSM::dcMappingHeuristicZigZagSM() :
		xRunnable(0), yRunnable(0), xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(
				0) {
}

/*
 * This simple mapping heuristic module
 * doesn't use all the informations provided.
 */
void dcMappingHeuristicZigZagSM::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicZigZagSM::addProcessingCore(const string& name, int x,
		int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void dcMappingHeuristicZigZagSM::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicZigZagSM::setApp(const string& appFile) {
}
void dcMappingHeuristicZigZagSM::completeRunnable(unsigned long int time,
		const string& runnableId) {
}
void dcMappingHeuristicZigZagSM::runnableMapped(unsigned long int time,
		const string& runnableId) {
}

}
}

