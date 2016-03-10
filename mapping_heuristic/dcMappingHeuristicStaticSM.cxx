/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#include "dcMappingHeuristicStaticSM.hxx"

#include <fstream>
#include <sstream>
#include <systemc.h>

namespace dreamcloud {
namespace platform_sclib {

/**
 * Map the labels in a zig zag fashion.
 * Map the runnables according to what is specified in file.
 *
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicStaticSM::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {
	dcMappingLocation loc = runMappings[runnableId].locs.at(
			runMappings[runnableId].i);
	return loc;
}

void dcMappingHeuristicStaticSM::runnableMapped(unsigned long int time,
		const string& runnableId) {
	runMappings[runnableId].i++;
}

/**
 * Map labels in a zig zag fashion.
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicStaticSM::mapLabel(
		const string& labelId, unsigned long int time,
		const string& labelName) {
	dcMappingLocation loc = dcMappingLocation(0, 0);
	return loc;
}

/**
 * Save the NoC size for labels and runnables mapping.
 */
void dcMappingHeuristicStaticSM::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicStaticSM::dcMappingHeuristicStaticSM() :
		xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(0) {
}

void dcMappingHeuristicStaticSM::setFile(string file) {
	ifstream f(file);
	string line;
	while (getline(f, line)) {
		stringstream lineStream(line);
		string id;
		getline(lineStream, id, ',');
		string name;
		getline(lineStream, name, ',');
		string pe;
		getline(lineStream, pe, ',');
		std::string half = pe.substr(0, pe.length() / 2);
		std::string otherHalf = pe.substr(pe.length() / 2);
		int x = std::stoi(half);
		int y = std::stoi(otherHalf);
		runMappings[id].name = name;
		runMappings[id].locs.push_back(dcMappingLocation(x, y));
	}
}

/*
 * This simple mapping heuristic module
 * doesn't use all the informations provided.
 */
void dcMappingHeuristicStaticSM::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicStaticSM::addProcessingCore(const string& name, int x,
		int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void dcMappingHeuristicStaticSM::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicStaticSM::setApp(const string& appFile) {
}
void dcMappingHeuristicStaticSM::completeRunnable(unsigned long int time,
		const string& runnableId) {
}

}
}
