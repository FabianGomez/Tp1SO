#ifndef __SCHED_RR__
#define __SCHED_RR__

#include <vector>
#include <queue>
#include "basesched.h"


using namespace std;


class SchedRR : public SchedBase {
	public:
		SchedRR(std::vector<int> argn);
        ~SchedRR();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		struct pcb 
		{
			int pid;
			State estado;

			pcb(): pid(IDLE_TASK), estado(Ready)
			{}

		};
		vector<int> quantums;
		vector<int> quantumsActuales;
		std::queue<pcb> cola;
		unsigned int siguiente();
		
};

#endif
