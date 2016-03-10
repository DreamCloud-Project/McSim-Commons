/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#include "dcMappingHeuristicMinComm.hxx"

#include <iostream>

using namespace std;

namespace dreamcloud {
namespace platform_sclib {

/**
 *  This mapper puts runnables where the biggest label
 *  that they read has been mapped. If such a label doesn't
 *  exist (the runnable doesn't read any label), it
 *  applies a zig zag strategy.
 *
 *  It maps labels in a zig zag order.
 *
 *  TAKE CARE: this mapper is dependent from the runnables order
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicMinComm::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {

	int x;
	int y;
	std::map<string, string>::const_iterator runToLabel =
			runnableIdToBiggestLabelId.find(runnableId);
	if (runToLabel != runnableIdToBiggestLabelId.end()) {
		string labelId = runToLabel->second;
		std::map<string, dcMappingLocation>::const_iterator labelToLoc =
				labelsMapping.find(labelId);
		if (labelToLoc == labelsMapping.end()) {
			cerr << " label" << labelId << " has not been mapped" << endl;
			exit(-1);
		}
		x = labelToLoc->second.first;
		y = labelToLoc->second.second;
	} else {
		x = xRunnable;
		y = yRunnable;
		yRunnable++;
		if (yRunnable == NoCYSize) {
			yRunnable = 0;
			xRunnable++;
			if (xRunnable == NoCXSize) {
				xRunnable = 0;
			}
		}
	}
	return dcMappingLocation(x, y);
}

/**
 * Map labels in a zig zag fashion.
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicMinComm::mapLabel(
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
	labelsMapping[labelId] = loc;
	return loc;
}

/**
 * Save the NoC size for labels and runnables mapping.
 */
void dcMappingHeuristicMinComm::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicMinComm::dcMappingHeuristicMinComm() :
		xRunnable(0), yRunnable(0), xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(
				0) {
}

void dcMappingHeuristicMinComm::setRunnables(
		vector<DCApplication::dcRunnableCall *> runnables) {
	for (std::vector<DCApplication::dcRunnableCall *>::const_iterator cit = runnables.begin();
			cit != runnables.end(); ++cit) {
		vector<string> reads = (*cit)->GetReadLabelsIdsBySize();
		if (!reads.empty()) {
			runnableIdToBiggestLabelId[(*cit)->GetRunClassId()] = reads[0];
		}
	}
}

/*
 * This simple mapping heuristic module
 * doesn't use all the informations provided.
 */
void dcMappingHeuristicMinComm::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicMinComm::addProcessingCore(const string& name, int x,
		int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void dcMappingHeuristicMinComm::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicMinComm::setApp(const string& appFile) {
}
void dcMappingHeuristicMinComm::completeRunnable(unsigned long int time,
		const string& runnableId) {
}
void dcMappingHeuristicMinComm::runnableMapped(unsigned long int time,
		const string& runnableId) {
}

}
}
