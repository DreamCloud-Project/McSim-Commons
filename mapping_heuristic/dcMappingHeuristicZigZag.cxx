/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#include "../../../Platform_Src/simulators-commons/mapping_heuristic/dcMappingHeuristicZigZag.hxx"

namespace dreamcloud {
namespace platform_sclib {

/**
 * Map the runnables and the labels in a zig zag fashion.
 *
 *  TAKE CARE: this mapper is dependent from the runnables order
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicZigZag::mapRunnable(
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
}

/**
 * Map labels in a zig zag fashion.
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicZigZag::mapLabel(
		const string& labelId, unsigned long int time,
		const string& labelName) {
	dcMappingLocation loc = dcMappingLocation(xLabel, yLabel);
	yLabel++;
	if (yLabel == NoCYSize) {
		yLabel = 0;
		xLabel++;
		if (xLabel == NoCXSize) {
			xLabel = 0;
		}
	}
	return loc;
}

/**
 * Save the NoC size for labels and runnables mapping.
 */
void dcMappingHeuristicZigZag::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicZigZag::dcMappingHeuristicZigZag() :
		xRunnable(0), yRunnable(0), xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(
				0) {
}

/*
 * This simple mapping heuristic module
 * doesn't use all the informations provided.
 */
void dcMappingHeuristicZigZag::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicZigZag::addProcessingCore(const string& name, int x,
		int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void dcMappingHeuristicZigZag::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicZigZag::setApp(const string& appFile) {
}
void dcMappingHeuristicZigZag::completeRunnable(unsigned long int time,
		const string& runnableId) {
}
void dcMappingHeuristicZigZag::runnableMapped(unsigned long int time,
		const string& runnableId) {
}
}
}
