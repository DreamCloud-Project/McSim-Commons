#ifndef DC_PERIODICEVENT
#define DC_PERIODICEVENT

#include <iostream>
#include <list>

#include "../../../platform/simulators-commons/parser/dcActEvent.h"

namespace DCApplication
{
   using namespace std;

   class dcPeriodicEvent : public dcActEvent {
		pair<int,string> Recurrence;
		pair<int,string> Offset;

	public:
		dcPeriodicEvent(string NameIn);
		~dcPeriodicEvent();
		void SetRecurrence(pair<int,string> RecurrenceIn);
		void SetOffset(pair<int,string> OffsetIn);
		pair<int,string> GetRecurrence();
		pair<int,string> GetOffset();
		void Print();
	}; 
}
#endif 