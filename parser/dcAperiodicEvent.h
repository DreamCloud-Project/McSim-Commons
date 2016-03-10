#ifndef AC_APERIODICEVENT_H
#define AC_APERIODICEVENT_H

#include <iostream>
#include <list>

#include "../../../Platform_Src/simulators-commons/parser/dcActEvent.h"

namespace DCApplication
{
   using namespace std;

   class dcAperiodicEvent : public dcActEvent {
	   string Val;

	public:
		dcAperiodicEvent(string NameIn);
		~dcAperiodicEvent();
		void SetVal(string ValIn);
		string GetVal();
		void Print();
	}; 
}

#endif // AC_APERIODICEVENT_H