#include "../../../Platform_Src/simulators-commons/parser/dcRunnableEdge.h"

namespace DCApplication
{
	using namespace std;
	dcRunnableEdge::dcRunnableEdge():ConnectsTo(NULL), Next(NULL), Tail(0){}

	void dcRunnableEdge::SetConnectTo(dcRunnableCall* ConnectToIn){
		ConnectsTo = ConnectToIn;
	}

	void dcRunnableEdge::SetNext(dcRunnableEdge* NextIn){
		Next = NextIn;
	}

	dcRunnableCall* dcRunnableEdge::GetConnectTo(){
		return ConnectsTo;
	}

	dcRunnableEdge* dcRunnableEdge::GetNext(){
		return Next;
	}

	void dcRunnableEdge::SetTail(dcRunnableEdge* TailIn){
		Tail = TailIn;
	}

	dcRunnableEdge* dcRunnableEdge::GetTail(){
		return Tail;
	}

	bool dcRunnableEdge::GetType(){
    	return Type;
	}

	void dcRunnableEdge::SetType(bool TypeIn){
	    Type = TypeIn;    
	}

}



