#include "uoyHeuristicModuleStatic.hxx"

namespace dreamcloud {
namespace platform_sclib {

uoyHeuristicModuleStatic::uoyHeuristicModuleStatic() {
}

void uoyHeuristicModuleStatic::AddMode(const string& ModeName) {
	Modes_.push_back(ModeName);
}

string uoyHeuristicModuleStatic::GetCurrentMode() {
	return CurrentMode_;
}

void uoyHeuristicModuleStatic::ClearModes() {
	Modes_.clear();
}

void uoyHeuristicModuleStatic::PrintModes() {
	for (auto Mode : Modes_) {
		cout << Mode << ' ';
	}
	cout << endl;
}

string uoyHeuristicModuleStatic::GetFutureMode() {
	return FutureMode_;
}

unsigned long int uoyHeuristicModuleStatic::GetSwitchingTime() {
	unsigned long int result;
	result = GetTimeOfModeChangeRequest();
	result += GetModeTransitionTimeFromSourceToDest(GetCurrentMode(),
			GetFutureMode());
	return result;
}

void uoyHeuristicModuleStatic::SetCurrentMode(const string& Mode) {
	try {
		for (auto MyMode : Modes_) {
			if (MyMode == Mode) {
				CurrentMode_ = Mode;
				return;
			}
		}
		throw std::runtime_error("Mode " + Mode + " not found");
	} catch (exception &e) {
		cerr << "Caught SetCurrentMode: " << e.what() << endl;
		exit(-1);
	};
}

void uoyHeuristicModuleStatic::ClearFutureMode() {
	SetFutureMode("");
}

void uoyHeuristicModuleStatic::AddMapForRunnableInModeItem(const string& Mode,
		const string& RunnableName, const int TargetCoreIndex) {
	MapForRunnableAndLabelInMode_[make_pair(RunnableName, Mode)] =
			TargetCoreIndex;
}

void uoyHeuristicModuleStatic::AddModeTransitionTimeFromSourceToDestItem(
		const string& PreviousModeName, const string& NextModeName,
		const unsigned long int TransitionTime, const string& TransitionUnit) {
	ModeTransitionTimeFromSourceToDest_[make_pair(PreviousModeName,
			NextModeName)] = make_pair(TransitionTime, TransitionUnit);
}

unsigned long int uoyHeuristicModuleStatic::GetModeTransitionTimeFromSourceToDest(
		const string& PreviousModeName, const string& NextModeName) {
	try {
		pair<unsigned long int, string> result;
		result = ModeTransitionTimeFromSourceToDest_.at(
				make_pair(PreviousModeName, NextModeName));
		return result.first;
	} catch (exception &e) {
		cerr << "Caught GetModeTransitionTimeFromSourceToDest: " << e.what()
				<< " while switching from " << PreviousModeName << " to "
				<< NextModeName << endl;
		exit(-1);
	};
	return 0;
}

void uoyHeuristicModuleStatic::SetFutureMode(const string& Mode) {
	FutureMode_ = Mode;
}

void uoyHeuristicModuleStatic::SetTimeOfModeChangeRequest(
		unsigned long int TimeOfModeChangeRequest) {
	TimeOfModeChangeRequest_ = TimeOfModeChangeRequest;
}

unsigned long int uoyHeuristicModuleStatic::GetTimeOfModeChangeRequest() {
	return TimeOfModeChangeRequest_;
}

int uoyHeuristicModuleStatic::GetMapForRunnableInModeItem(const string& Mode,
		const string& RunnableName) {
	try {
		int result = MapForRunnableAndLabelInMode_.at(
				std::make_pair(RunnableName, Mode));
		return result;
	} catch (exception &e) {
		cerr << "Caught GetMapForRunnableInModeItem: " << e.what() << " mode = "
				<< Mode << " and runnable/label name = " << RunnableName
				<< endl;
		exit(-1);
	};
	return -1;
}

void uoyHeuristicModuleStatic::ParseModeFile(const string& strDirectory,
		const string& strName) {
	string Line;
	string Mode = RemoveExtension(strName);
	unsigned int TargetCoreIndex;
	string RunnableName;

	std::smatch res;
	std::regex rx("^([A-Za-z0-9_]+),([0-9]+)$");	//Name,CoreIndex

	ifstream inFile(strDirectory + "/" + strName);
	if (inFile.is_open()) {
		while (inFile.good()) {
			getline(inFile, Line);
			if (regex_match(Line, res, rx)) {
				std::ssub_match sub_match1 = res[1];
				RunnableName = sub_match1.str();
				std::ssub_match sub_match2 = res[2];
				string TargetCoreIndexStr = sub_match2.str();
				TargetCoreIndex = std::stoi(TargetCoreIndexStr);
				AddMapForRunnableInModeItem(Mode, RunnableName,
						TargetCoreIndex);
			} else {
				if (Line.length() > 0) {
					cerr << "Syntax error" << std::endl << std::endl;
					exit(-1);
				}
			}

		}
		inFile.close();

	} else {
		cerr << "Unable to open mode file " << strDirectory + "/" + strName
				<< std::endl << std::endl;
		exit(-1);
	}
}

void uoyHeuristicModuleStatic::ParseModeChangeTiming(
		const string& strDirectory) {
	string Line;
	std::smatch res;
	std::regex rx("^([A-Za-z0-9_]+),([A-Za-z0-9_]+),([0-9]+)([mnup]s)$");//Name,CoreIndex

	string PreviousModeName, NextModeName, TransitionUnit;
	int TransitionTime;

	ifstream inFile(strDirectory + "/ModeChangeTiming.csv");
	if (inFile.is_open()) {
		while (inFile.good()) {
			getline(inFile, Line);

			if (regex_match(Line, res, rx)) {
				std::ssub_match sub_match1 = res[1];
				PreviousModeName = sub_match1.str();
				std::ssub_match sub_match2 = res[2];
				NextModeName = sub_match2.str();
				std::ssub_match sub_match3 = res[3];
				string TransitionTimeStr = sub_match3.str();
				TransitionTime = std::stoi(TransitionTimeStr);
				std::ssub_match sub_match4 = res[4];
				TransitionUnit = sub_match4.str();
				AddModeTransitionTimeFromSourceToDestItem(PreviousModeName,
						NextModeName, TransitionTime, TransitionUnit);
			} else {
				if (Line.length() > 0) {
					cerr << "Syntax error" << std::endl << std::endl;
					exit(-1);
				}
			}
		}
		inFile.close();
	} else {
		cerr << "Unable to open file" << std::endl << std::endl;
		exit(-1);
	}
}

void uoyHeuristicModuleStatic::ParseModeListFile(const string& strDirectory) {

	string Line;

	ifstream inFile(strDirectory + "/ModeList.csv");
	if (inFile.is_open()) {
		while (inFile.good()) {
			getline(inFile, Line);
			if (!Line.empty()) {
				AddMode(Line);
			}
		}
		inFile.close();

		for (auto Mode : Modes_) {
			ParseModeFile(strDirectory, Mode + ".csv");
		}

	} else {
		cerr << "Unable to open file" << std::endl << std::endl;
		exit(-1);
	}
}

string uoyHeuristicModuleStatic::RemoveExtension(const string& FileName) {
	size_t LastDot = FileName.find_last_of(".");
	if (LastDot == std::string::npos)
		return FileName;
	return FileName.substr(0, LastDot);
}

void uoyHeuristicModuleStatic::SwitchModeIfRequired(
		unsigned long int CurrentTime) {
	if (GetFutureMode().length() > 0 && CurrentTime >= GetSwitchingTime()) {
		SetCurrentMode(GetFutureMode());
		ClearFutureMode();
	}
}

/**
 * Cores numbers regarding the 3core architecture
 * represented by a 3x2 NoC
 *
 * 0 - CPU0    - 0,0
 * 1 - CPU1    - 1,0
 * 2 - CPU2    - 2,0
 * 3 - PFlash0 - 0,1
 * 4 - PFlash1 - 0,2
 */
dcMappingHeuristicI::dcMappingLocation uoyHeuristicModuleStatic::indexToLocation(
		int index) {
	if (index == 0) {
		return dcMappingLocation(0, 0);
	} else if (index == 1) {
		return dcMappingLocation(1, 0);
	} else if (index == 2) {
		return dcMappingLocation(2, 0);
	} else if (index == 3) {
		return dcMappingLocation(0, 1);
	} else if (index == 4) {
		return dcMappingLocation(0, 2);
	} else {
		cerr << "unknown index in UoY mapping heuristic" << endl;
		exit(-1);
	}
}

//////////////////////////////////////////////
///// Interface implementation ///////////////
//////////////////////////////////////////////

dcMappingHeuristicI::dcMappingLocation uoyHeuristicModuleStatic::mapRunnable(
		unsigned long int time, const string& runnableId,
		const string& runnableName, const string& taskName,
		const string& taskId, unsigned int idInTask, unsigned int periodId) {
	SwitchModeIfRequired(time);
	return indexToLocation(
			GetMapForRunnableInModeItem(GetCurrentMode(), runnableName));
}

dcMappingHeuristicI::dcMappingLocation uoyHeuristicModuleStatic::mapLabel(
		const string& labelId, unsigned long int time,
		const string& labelName) {
	SwitchModeIfRequired(time);
	return indexToLocation(
			GetMapForRunnableInModeItem(GetCurrentMode(), labelName));
}

void uoyHeuristicModuleStatic::switchMode(unsigned long int time,
		const string& newModeFile, const string& newModeName) {
	if (CurrentMode_.empty()) {
		SetCurrentMode(newModeName);
	} else {
		SetFutureMode(newModeName);
		SetTimeOfModeChangeRequest(time);
	}
}

void uoyHeuristicModuleStatic::setNocSize(unsigned int NoCXSize,
		unsigned int NoCYSize) {
}
void uoyHeuristicModuleStatic::addCoreType(const string& coreType,
		double energyMultiplier, double switchingPModeTimePenalty,
		double switchingPModeEnergyPenalty, int maxFrequencyInHz,
		map<double, double> voltageToFreq, int voltToFreqLevel) {
}
void uoyHeuristicModuleStatic::addProcessingCore(const string& name, int x,
		int y, const string& coreType, unsigned int clockRatio,
		unsigned int quartz, unsigned int ticksPerCycle,
		double staticEnergyValue, const string& staticEnergyUnit,
		double energyMultiplier, int pMode) {
}
void uoyHeuristicModuleStatic::setApp(const string& appFile) {
}
void uoyHeuristicModuleStatic::completeRunnable(unsigned long int time,
		const string& runnableId) {
}
void uoyHeuristicModuleStatic::runnableMapped(unsigned long int time,
		const string& runnableId) {
}

}
}

