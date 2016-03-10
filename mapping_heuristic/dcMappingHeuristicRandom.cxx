/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#include "../../../Platform_Src/simulators-commons/mapping_heuristic/dcMappingHeuristicRandom.hxx"

#include <string.h>
#include <fstream>
#include <sstream>

namespace dreamcloud {
namespace platform_sclib {

dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicRandom::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {

	int xLowerBound = 0;
	int xUpperBound = NoCXSize - 1;
	std::uniform_int_distribution<> distrx(xLowerBound, xUpperBound);
	int xRunnable = distrx(gen);

	int yLowerBound = 0;
	int yUpperBound = NoCYSize - 1;
	std::uniform_int_distribution<> distry(yLowerBound, yUpperBound);
	int yRunnable = distry(gen);

	return dcMappingLocation(xRunnable, yRunnable);;

}

/**
 * Map labels in a zig zag fashion.
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicRandom::mapLabel(
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
void dcMappingHeuristicRandom::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicRandom::dcMappingHeuristicRandom(unsigned int seed) :
		xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(0) {
	//Set-up the PRNG
	gen = std::mt19937(seed);
}

/*
 * This simple mapping heuristic module
 * doesn't use all the informations provided.
 */
void dcMappingHeuristicRandom::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicRandom::addProcessingCore(const string& name, int x,
		int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void dcMappingHeuristicRandom::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicRandom::setApp(const string& appFile) {
}
void dcMappingHeuristicRandom::completeRunnable(unsigned long int time,
		const string& runnableId) {
}
void dcMappingHeuristicRandom::runnableMapped(unsigned long int time,
		const string& runnableId) {
}
}
}
