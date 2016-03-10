#include "../../../Platform_Src/simulators-commons/parser/dcExecutionCyclesConstantInstruction.h"

#include "../../../Platform_Src/simulators-commons/parser/dcInstruction.h"


namespace DCApplication
{
	dcExecutionCyclesConstantInstruction::dcExecutionCyclesConstantInstruction(string NameIn): dcInstruction(NameIn), Value(0) {
	}

	int dcExecutionCyclesConstantInstruction::GetValue() {
		return Value;
	}

    void dcExecutionCyclesConstantInstruction::SetValue(int ValueIn) {
		Value=ValueIn;
	}

	void dcExecutionCyclesConstantInstruction::Print() {
		cout << GetName() ;
	}
}