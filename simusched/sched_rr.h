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
			int core;
			State estado;

			pcb(): pid(IDLE_TASK), core(-1), estado(Ready)
			{}

		};
		vector<int> quantums;
		vector<int> quantumsActuales;
		std::queue<pcb> cola;
		vector<pcb> procesosActuales; //la pos i tiene el actual del core i
};

#endif
