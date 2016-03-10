/*
 * dcMappingHeuristic.cxx
 *
 *  Created on: Sep 17, 2015
 *      Author: manu
 */
#include "dcMappingHeuristicKhalidDC.hxx"
#include <iostream>

namespace dreamcloud {
namespace platform_sclib {

/**
 * Map the runnables according to what Khalid did for DC
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicKahlidDC::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {

	if (runnableId == "_H08ZUN9NEeOETou3oNmhPQ") { //CheckPlausability
		return dcMappingLocation(3, 3);
	}
	if (runnableId == "_H08Y7t9NEeOETou3oNmhPQ") { //BrakeActuatorMonitor
		return dcMappingLocation(2, 2);
	}
	if (runnableId == "_H08ZgN9NEeOETou3oNmhPQ") { //DiagnosisArbiter") {
		return dcMappingLocation(1, 3);
	}
	if (runnableId == "_H08aRt9NEeOETou3oNmhPQ") { //VehicleStateMonitor") {
		return dcMappingLocation(2, 0);
	}
	if (runnableId == "_H08ZDN9NEeOETou3oNmhPQ") { //BrakeForceCalculation") {
		return dcMappingLocation(1, 3);
	}
	if (runnableId == "_H08ZON9NEeOETou3oNmhPQ") { //BrakeSafetyMonitor") {
		return dcMappingLocation(1, 3);
	}
	if (runnableId == "_H08Yud9NEeOETou3oNmhPQ") { //ABSCalculation") {
		return dcMappingLocation(1, 2);
	}
	if (runnableId == "_H08Y999NEeOETou3oNmhPQ") { //BrakeForceActuation") {
		return dcMappingLocation(0, 0);
	}
	if (runnableId == "_H08ZRt9NEeOETou3oNmhPQ") { //CaliperPositionCalculation") {
		return dcMappingLocation(2, 3);
	}
	if (runnableId == "_H08Y5d9NEeOETou3oNmhPQ") { //BrakeActuator") {
		return dcMappingLocation(1, 3);
	}
	if (runnableId == "_H08Z8N9NEeOETou3oNmhPQ") { //StopLightActuator") {
		return dcMappingLocation(1, 2);
	}
	if (runnableId == "_H08ZGt9NEeOETou3oNmhPQ") { //BrakePedalSensorDiagnosis") {
		return dcMappingLocation(0, 2);
	}
	if (runnableId == "_H08ZI99NEeOETou3oNmhPQ") { //BrakePedalSensorTranslation") {
		return dcMappingLocation(0, 2);
	}
	if (runnableId == "_H08ZLt9NEeOETou3oNmhPQ") { //BrakePedalSensorVoter") {
		return dcMappingLocation(0, 0);
	}
	if (runnableId == "_H08ZYt9NEeOETou3oNmhPQ") { //DecelerationSensorDiagnosis") {
		return dcMappingLocation(2, 3);
	}
	if (runnableId == "_H08Za99NEeOETou3oNmhPQ") { //DecelerationSensorTranslation") {
		return dcMappingLocation(2, 3);
	}
	if (runnableId == "_H08aKN9NEeOETou3oNmhPQ") { //VehicleSpeedSensorDiagnosis") {
		return dcMappingLocation(3, 0);
	}
	if (runnableId == "_H08Zdt9NEeOETou3oNmhPQ") { //DecelerationSensorVoter") {
		return dcMappingLocation(2, 1);
	}
	if (runnableId == "_H08aPN9NEeOETou3oNmhPQ") { //VehicleSpeedSensorVoter") {
		return dcMappingLocation(2, 2);
	}
	if (runnableId == "_H08aMd9NEeOETou3oNmhPQ") { //VehicleSpeedSensorTranslation") {
		return dcMappingLocation(3, 0);
	}
	if (runnableId == "_H08aV99NEeOETou3oNmhPQ") { //WheelSpeedSensorTranslation") {
		return dcMappingLocation(1, 1);
	}
	if (runnableId == "_H08aTt9NEeOETou3oNmhPQ") { //WheelSpeedSensorDiagnosis") {
		return dcMappingLocation(1, 1);
	}
	if (runnableId == "_H08ZWd9NEeOETou3oNmhPQ") { //CylNumObserver") {
		return dcMappingLocation(2, 0);
	}
	if (runnableId == "_H08aYt9NEeOETou3oNmhPQ") { //WheelSpeedSensorVoter") {
		return dcMappingLocation(0, 3);
	}
	if (runnableId == "_H08aAt9NEeOETou3oNmhPQ") { //ThrottleController") {
		return dcMappingLocation(3, 2);
	}
	if (runnableId == "_H08Y0d9NEeOETou3oNmhPQ") { //APedVoter") {
		return dcMappingLocation(0, 2);
	}
	if (runnableId == "_H08Y299NEeOETou3oNmhPQ") { //BaseFuelMass") {
		return dcMappingLocation(0, 2);
	}
	if (runnableId == "_H08Z-d9NEeOETou3oNmhPQ") { //ThrottleActuator") {
		return dcMappingLocation(3, 1);
	}
	if (runnableId == "_H08aFt9NEeOETou3oNmhPQ") { //TotalFuelMass") {
		return dcMappingLocation(2, 0);
	}
	if (runnableId == "_H08aH99NEeOETou3oNmhPQ") { //TransientFuelMass") {
		return dcMappingLocation(2, 0);
	}
	if (runnableId == "_H08Z1t9NEeOETou3oNmhPQ") { //InjectionTimeActuation") {
		return dcMappingLocation(1, 3);
	}
	if (runnableId == "_H08Zzd9NEeOETou3oNmhPQ") { //IgnitionTiming") {
		return dcMappingLocation(0, 0);
	}
	if (runnableId == "_H08ZvN9NEeOETou3oNmhPQ") { //IgnitionTimeActuation") {
		return dcMappingLocation(3, 3);
	}
	if (runnableId == "_H08ZAt9NEeOETou3oNmhPQ") { //BrakeForceArbiter") {
		return dcMappingLocation(1, 1);
	}
	if (runnableId == "_H08ZiN9NEeOETou3oNmhPQ") { //EcuBrakeActuator") {
		return dcMappingLocation(3, 1);
	}
	if (runnableId == "_H08Zot9NEeOETou3oNmhPQ") { //EcuStopLightActuator") {
		return dcMappingLocation(2, 1);
	}
	if (runnableId == "_H08ZkN9NEeOETou3oNmhPQ") { //EcuBrakePedalSensor") {
		return dcMappingLocation(2, 2);
	}
	if (runnableId == "_H08Zmd9NEeOETou3oNmhPQ") { //EcuDecelerationSensor") {
		return dcMappingLocation(2, 3);
	}
	if (runnableId == "_H08Zqt9NEeOETou3oNmhPQ") { //EcuVehicleSpeedSensor") {
		return dcMappingLocation(3, 0);
	}
	if (runnableId == "_H08Zs99NEeOETou3oNmhPQ") { //EcuWheelSpeedSensor") {
		return dcMappingLocation(3, 2);
	}
	if (runnableId == "_H08Yxt9NEeOETou3oNmhPQ") { //APedSensor") {
		return dcMappingLocation(1, 0);
	}
	if (runnableId == "_H08aDN9NEeOETou3oNmhPQ") { //ThrottleSensor") {
		return dcMappingLocation(1, 1);
	}
	if (runnableId == "_H08Z599NEeOETou3oNmhPQ") { //MassAirFlowSensor") {
		return dcMappingLocation(0, 1);
	}
	std::cerr << "Invalid runnable id " << runnableId
			<< " in dcMappingHeuristicKhalidDC";
	exit(-1);
}

/**
 * Map labels in a simple round robin fashion.
 */
dcMappingHeuristicI::dcMappingLocation dcMappingHeuristicKahlidDC::mapLabel(
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
void dcMappingHeuristicKahlidDC::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
	this->NoCXSize = NoCXSize;
	this->NoCYSize = NoCYSize;
}

/**
 * Constructor
 */
dcMappingHeuristicKahlidDC::dcMappingHeuristicKahlidDC() :
		xLabel(0), yLabel(0), NoCXSize(0), NoCYSize(0) {
}

/*
 * This simple mapping heuristic module
 * doesn't use all the informations provided.
 */
void dcMappingHeuristicKahlidDC::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void dcMappingHeuristicKahlidDC::addProcessingCore(const string& name, int x,
		int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void dcMappingHeuristicKahlidDC::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
}
void dcMappingHeuristicKahlidDC::setApp(const string& appFile) {
}
void dcMappingHeuristicKahlidDC::completeRunnable(unsigned long int time,
		const string& runnableId) {
}
void dcMappingHeuristicKahlidDC::runnableMapped(unsigned long int time,
		const string& runnableId) {
}
}
}
