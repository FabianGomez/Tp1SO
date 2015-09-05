#ifndef __SCHED_RR2__
#define __SCHED_RR2__

#include <vector>
#include <queue>
#include "basesched.h"

using namespace std;


class SchedRR2 : public SchedBase {
	public:
		SchedRR2(std::vector<int> argn);
        ~SchedRR2();
		virtual void load(int pid);
		virtual void unblock(int pid);
		virtual int tick(int cpu, const enum Motivo m);

	private:
		struct pcb 
		{
			int pid;
			int core;
			State estado;

			pcb(): pid(IDLE_TASK), core(-1), estado(Ready)
			{}

		};
		vector<int> quantums;
		vector<int> quantumsActuales;
		
		vector< std::queue<pcb> > colasxCore;
		

		vector<pcb> procesosActuales; //la pos i tiene el actual del core i
};

#endif
