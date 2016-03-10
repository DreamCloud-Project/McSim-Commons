#ifndef DC_DETERMINISTICIRREGULAREVENT_H
#define DC_DETERMINISTICIRREGULAREVENT_H

#include <iostream>
#include <list>
#include <iostream>
#include <list>

#include "../../../platform/simulators-commons/parser/dcActEvent.h"
#include "../../../platform/simulators-commons/parser/dcActEvent.h"

namespace DCApplication
{
   using namespace std;

   class dcDeterministicIrregularEvent : public dcActEvent {
	   string NameIn;
	   vector<string> ArrivalTimes;

	public:
		dcDeterministicIrregularEvent(string NameIn);
		~dcDeterministicIrregularEvent();
		void SetArrivalTimes(vector<string> ArrivalTimesIn);
		void AddArrivalTimes(string ArrivalTimes);
		vector<string> GetArrivalTimes();
		void Print();
	}; 
}

#endif // DC_DETERMINISTICIRREGULAREVENT_H