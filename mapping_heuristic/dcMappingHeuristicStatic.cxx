/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#include "dcMappingHeuristicStatic.hxx"

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
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicStatic::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {

	dcMappingLocation loc = runMappings[runnableId].locs.at(
			runMappings[runnableId].i);
	return loc;
}

void dcMappingHeuristicStatic::runnableMapped(unsigned long int time,
		const string& runnableId) {
	runMappings[runnableId].i++;
}

/**
 * Map labels in a zig zag fashion.
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicStatic::mapLabel(
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
void dcMappingHeuristicStatic::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicStatic::dcMappingHeuristicStatic() :
		xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(0) {
}

void dcMappingHeuristicStatic::setFile(string file) {
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
void dcMappingHeuristicStatic::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicStatic::addProcessingCore(const string& name, int x,
		int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void dcMappingHeuristicStatic::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicStatic::setApp(const string& appFile) {
}
void dcMappingHeuristicStatic::completeRunnable(unsigned long int time,
		const string& runnableId) {
}

}
}
