#ifndef __SCHED_NOMIS__
#define __SCHED_NOMIS__

#include <vector>
#include <deque>
#include "basesched.h"
#include <map>

class SchedNoMistery : public SchedBase {
  public:
    SchedNoMistery(std::vector<int> argn);
    virtual void load(int pid);
    virtual void unblock(int pid);
    virtual int tick(int cpu, const enum Motivo m);  

  private:
  	struct pcb;

	std::vector<int> quantums; //Los quantums de entrada
	std::deque<pcb> cola; //Procesos que no se estan ejecutando
	unsigned int index; //Indice de quantums
	unsigned int procesos; //Cantidad de procesos a ejecutar(no Bloqueados, Ready)
	unsigned int quantumActual; //Ticks restantes del proceso actual
	std::map<unsigned int,unsigned int> quantumBloqueados;
	void cambiarQuantum(unsigned int pid);
  	struct pcb 
	{
		int pid;
		State estado;

		pcb(): pid(IDLE_TASK), estado(Ready)
		{}

	};


};

#endif
