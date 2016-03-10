/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#include "../../../Platform_Src/simulators-commons/mapping_heuristic/dcMappingHeuristicStaticTriCore.hxx"

#include <fstream>
#include <sstream>
#include <systemc.h>

namespace dreamcloud {
namespace platform_sclib {

dcMappingHeuristicStaticTriCore::RunMapping dcMappingHeuristicStaticTriCore::findMapping(
		string runnableId, string runnableName, string taskName, string taskId,
		unsigned int idInTask) {
	vector<dcMappingHeuristicStaticTriCore::RunMapping>::iterator it =
			std::find_if(runMappings.begin(), runMappings.end(),
					[&runnableId, &runnableName, &taskName, &taskId,
					&idInTask](const dcMappingHeuristicStaticTriCore::RunMapping& m)
					{
						return m.runnableId == runnableId &&
						m.idInTask == idInTask &&
						m.runnableName == runnableName &&
						m.taskId==taskId;
					});
	if (it != runMappings.end()) {
		return *it;
	} else {
		cerr << "Invalid runnable:" << endl;
		cerr << " run id = " << runnableId << endl;
		cerr << " run name = " << runnableName << endl;
		cerr << " task id = " << taskId << endl;
		cerr << " task name = " << taskName << endl;
		cerr << " id in task = " << idInTask << endl;
		cerr << "in static mapping TriCore" << endl;
		exit(-1);
	}
}

/**
 * Map the labels in a zig zag fashion.
 * Map the runnables according to what is specified in file.
 *
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicStaticTriCore::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {

	RunMapping m = findMapping(runnableId, runnableName, taskName, taskId,
			idInTask);
	return m.loc;
}

/**
 * Map labels in a zig zag fashion.
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicStaticTriCore::mapLabel(
		const string& labelId, unsigned long int time,
		const string& labelName) {
	dcMappingLocation loc = dcMappingLocation(xLabel, yLabel);
	xLabel++;
	if (xLabel == 2) {
		xLabel = 0;
	}
	return loc;
}

/**
 * Save the NoC size for labels and runnables mapping.
 */
void dcMappingHeuristicStaticTriCore::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicStaticTriCore::dcMappingHeuristicStaticTriCore() :
		xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(0) {
}

void dcMappingHeuristicStaticTriCore::setFile(string file) {
	ifstream f(file);
	string line;
	while (getline(f, line)) {
		stringstream lineStream(line);
		string runId;
		getline(lineStream, runId, ',');
		string runName;
		getline(lineStream, runName, ',');
		string taskId;
		getline(lineStream, taskId, ',');
		string taskName;
		getline(lineStream, taskName, ',');
		string idInTaskS;
		getline(lineStream, idInTaskS, ',');
		string pe;
		getline(lineStream, pe, ',');
		std::string half = pe.substr(0, pe.length() / 2);
		std::string otherHalf = pe.substr(pe.length() / 2);
		int x = std::stoi(half);
		int y = std::stoi(otherHalf);
		RunMapping rm;
		rm.runnableId = runId;
		rm.runnableName = runName;
		rm.taskId = taskId;
		rm.taskName = taskName;
		rm.idInTask = std::stoi(idInTaskS);
		rm.loc = dcMappingHeuristicI::dcMappingLocation(x,y);
		runMappings.push_back(rm);
	}
}

/*
 * This simple mapping heuristic module
 * doesn't use all the informations provided.
 */
void dcMappingHeuristicStaticTriCore::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicStaticTriCore::addProcessingCore(const string& name,
		int x, int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void dcMappingHeuristicStaticTriCore::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicStaticTriCore::setApp(const string& appFile) {
}
void dcMappingHeuristicStaticTriCore::completeRunnable(unsigned long int time,
		const string& runnableId) {
}
void dcMappingHeuristicStaticTriCore::runnableMapped(unsigned long int time,
		const string& runnableId) {
}

}
}
