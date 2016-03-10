/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#include "dcMappingHeuristicWeka.hxx"

#include <fstream>
#include <sstream>
#include <systemc.h>

namespace dreamcloud {
namespace platform_sclib {

/**
 * Map the labels in a zig zag fashion.
 * Map the runnables according to what is specified in two files
 * (static and random) in the following way:
 *
 * - search for the runnable to map in the random file
 * - if found, map it randomly
 * - else map it according to what is specified in the static file
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicWeka::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {

	// If random mapping
	if (std::find(randomRuns.begin(), randomRuns.end(), runnableId)
			!= randomRuns.end()) {
		dcMappingLocation loc = randomMappings[runnableId].locs.at(
				randomMappings[runnableId].i);
		return loc;
	}

	// If static mapping
	else if (std::find(staticRuns.begin(), staticRuns.end(), runnableId)
			!= staticRuns.end()) {
		dcMappingLocation loc = staticMappings[runnableId].locs.at(
				staticMappings[runnableId].i);
		return loc;
	}

	else {
		std::cerr << " no mapping for runnable " << runnableId << std::endl;
		exit(-1);
	}
}

void dcMappingHeuristicWeka::runnableMapped(unsigned long int time,
		const string& runnableId) {

	// If random mapping
	if (std::find(randomRuns.begin(), randomRuns.end(), runnableId)
			!= randomRuns.end()) {
		randomMappings[runnableId].i++;
	}

	// If static mapping
	else if (std::find(staticRuns.begin(), staticRuns.end(), runnableId)
			!= staticRuns.end()) {
		staticMappings[runnableId].i++;
	}

	else {
		std::cerr << " no mapping for runnable " << runnableId << std::endl;
		exit(-1);
	}
}

/**
 * Map labels in a zig zag fashion.
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicWeka::mapLabel(
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
void dcMappingHeuristicWeka::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicWeka::dcMappingHeuristicWeka(int seed) :
		xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(0) {
	gen = std::mt19937(seed);
}

void dcMappingHeuristicWeka::setFiles(string staticFile, string randomFile) {

	// Static mappings
	ifstream f(staticFile);
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
		staticMappings[id].name = name;
		staticMappings[id].locs.push_back(dcMappingLocation(x, y));
		staticRuns.push_back(id);
	}
	f.close();

	// Random mappings
	string id;
	ifstream rf(randomFile);
	while (getline(rf, id)) {
		std::uniform_int_distribution<> distrx(0, NoCXSize - 1);
		int x = distrx(gen);
		std::uniform_int_distribution<> distry(0, NoCYSize - 1);
		int y = distry(gen);
		dcMappingLocation loc = dcMappingLocation(x, y);
		randomMappings[id].name = "";
		randomMappings[id].locs.push_back(loc);
		randomRuns.push_back(id);
	}
	rf.close();
}

/*
 * This simple mapping heuristic module
 * doesn't use all the informations provided.
 */
void dcMappingHeuristicWeka::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicWeka::addProcessingCore(const string& name, int x, int y,
		const string& coreType, unsigned int clockRatio, unsigned int quartz,
		unsigned int ticksPerCycle, double staticEnergyValue,
		const string& staticEnergyUnit, double energyMultiplier, int pMode) {
}
void dcMappingHeuristicWeka::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicWeka::setApp(const string& appFile) {
}
void dcMappingHeuristicWeka::completeRunnable(unsigned long int time,
		const string& runnableId) {
}

}
}
