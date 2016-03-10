#ifndef RUNNABLELABEL_H_
#define RUNNABLELABEL_H_


#include <iostream>
#include <string>

namespace DCApplication{
	using namespace std;
	class dcRunnableCall;
	class dcRunnableEdge
	{
		public:
			dcRunnableEdge();
			void SetConnectTo(dcRunnableCall* ConnectToIn);
			void SetNext(dcRunnableEdge* NextIn);
			dcRunnableCall* GetConnectTo();
			dcRunnableEdge* GetNext();
			dcRunnableEdge* GetTail();
			void SetTail(dcRunnableEdge* TailIn);
			bool GetType();
			void SetType(bool TypeIn);

		private:
			dcRunnableCall *ConnectsTo;
			dcRunnableEdge *Next;
			dcRunnableEdge *Tail;
			bool Type;
	};
}



#endif /* RUNNABLELABEL_H_ */