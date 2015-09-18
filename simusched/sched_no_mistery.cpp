#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {  
	quantums = vector<int>(argn);
	colas = vector<queue<pcb> > (argn.size());
	indices = vector<int>();
	quantumsActuales = vector<int>();
}

void SchedNoMistery::load(int pid) {  
	//creo una nueva pcb para el proceso

	pcb p = pcb();
	p.pid = pid;

	// le asignamos
	indices.push_back(0);
	quantumsActuales.push_back(quantums[0]);
	colas[0].push(p);
}

void SchedNoMistery::unblock(int pid) 
{
    pcb p = pcb();
    p.pid = pid; // esta en ready incialmente.

    // gana maxima prioridad.
    int indice = indices[pid] == 0 ? 0 : indices[pid]-1;
    indices[pid] = indice;
    colas[indice].push(p);
    quantumsActuales[pid] = quantums[indice];
}

int SchedNoMistery::proximaColaConElementos()
{
	int idx = 0;
	while(idx < colas.size() && colas[idx].size() == 0)
	{
		idx++;
	}
	return idx;
}

int SchedNoMistery::proxAEjecutar()
{
	int idx = proximaColaConElementos();

	if (idx == colas.size())
	{
		return IDLE_TASK;
	}
	else
	{
		pcb siguiente = colas[idx].front();
		colas[idx].pop();
		return siguiente.pid;
	}
}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  

	if (m == EXIT || m == BLOCK)
	{
		// busco el siguiente a correr
		return proxAEjecutar();
	}
	else
	{
		int pid = current_pid(cpu);

		if (pid == IDLE_TASK)
		{
			return proxAEjecutar();
		}

		quantumsActuales[pid]--;
		int q = quantumsActuales[pid];
		if( q == 0 )
		{
			// me tengo que pushear en la proxima cola
			int indice = indices[pid] < colas.size()-1 ? indices[pid]+1 : indices[pid];
			indices[pid] = indice;
			quantumsActuales[pid] = quantums[indice];
			pcb yo = pcb();
			yo.pid = pid;
			colas[indice].push(yo);
			return proxAEjecutar();

		}
		else
			return current_pid(cpu);
	}

	return 0;
}

