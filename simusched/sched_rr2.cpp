#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	int n = argn[0];
	quantums = vector<int>(n,0);
	quantumsActuales = vector<int>(n,0);
	colasxCore = vector< std::queue<pcb> >(n);
	for(int i =1;i<n+1;i++){
		quantums[i-1] = argn[i];
		quantumsActuales[i-1] = argn[i];
		colasxCore[i-1] = std::queue<pcb>();
	}

	procesosActuales = vector<pcb>(n, pcb());
}

SchedRR2::~SchedRR2() {

}


void SchedRR2::load(int pid) {

	int min=colasxCore[0].size();
	int minindex=0;
	for(int i=1;i<colasxCore.size();i++){
		if(colasxCore[i].size() < min){
			min=colasxCore[i].size() ;
			minindex=i;
		}
	}
	pcb p = pcb();
	p.pid = pid;
	colasxCore[minindex].push(p);

}

void SchedRR2::unblock(int pid) {
}

int SchedRR2::tick(int cpu, const enum Motivo m) {
// EN COLA NO SON LOS QUE SE ESTAN EJECUTANDO
	// PARA ESO ESTA procesosActuales!!!!!

	if( m == EXIT || m == BLOCK) 
	{
		// reseteamos el quantum
		quantumsActuales[cpu] = quantums[cpu];

		// sobre el que decido.
		pcb primero = procesosActuales[cpu];

		// se contempla tambien el caso vacio
		unsigned int i = 0;
		while(colasxCore[cpu].front().estado == Blocked && i < colasxCore[cpu].size())
		{
			pcb p = colasxCore[cpu].front();colasxCore[cpu].pop();
			colasxCore[cpu].push(p);
			i++;
		}

		// me vuelvo a pushear, si me bloque
		if (m == BLOCK)
			colasxCore[cpu].push(primero);

		// reseteo
		procesosActuales[cpu] = pcb();

		if(i == colasxCore[cpu].size())
			return IDLE_TASK;
		else
		{
			procesosActuales[cpu] = colasxCore[cpu].front();
			colasxCore[cpu].pop();
			return procesosActuales[cpu].pid;
		}

	} else if (m == TICK)
	{
		

		quantumsActuales[cpu]--; //Si estoy en la idle me puede dar negativo, pero no importa porque lo piso
		if (colasxCore[cpu].size() == 0 && current_pid(cpu) == IDLE_TASK)
			//No hay nadie para correr, sigo con la idle
			return current_pid(cpu);
		else if(colasxCore[cpu].size() > 0 && current_pid(cpu) == IDLE_TASK)
		{
			//Hay alguien en la cola. Tengo que ver si hay alguien ready
			int i = 0;
			while(colasxCore[cpu].front().estado == Blocked && i < colasxCore[cpu].size())
			{
				pcb p = colasxCore[cpu].front();
				colasxCore[cpu].pop();
				colasxCore[cpu].push(p);
				i++;
			}
			if(i == colasxCore[cpu].size())
				//No hay nadie ready, le toca al proceso actual de nuevo
				return current_pid(cpu);
			else
			{
				quantumsActuales[cpu] = quantums[cpu];
				procesosActuales[cpu] = colasxCore[cpu].front();
				colasxCore[cpu].pop();
				return procesosActuales[cpu].pid;
			}
		}
		else if(colasxCore[cpu].size() == 0 && current_pid(cpu) != IDLE_TASK)
		{
			//No hay nadie esperando, solo tengo que verificar si hay que resetear el quantum

			if(quantumsActuales[cpu] == 0)
				quantumsActuales[cpu] = quantums[cpu];
			return current_pid(cpu);
		}
		else
		{
			//Hay alguien en la cola y no estoy corriendo la idle.
			if(quantumsActuales[cpu] == 0)
			{
				int i = 0;
				while(colasxCore[cpu].front().estado == Blocked && i < colasxCore[cpu].size())
				{
					pcb p = colasxCore[cpu].front();
					colasxCore[cpu].pop();
					colasxCore[cpu].push(p);
					i++;
				}
				quantumsActuales[cpu] = quantums[cpu];
				if(i == colasxCore[cpu].size())
					//No hay nadie mas ready, le toca al proceso actual de nuevo
					return current_pid(cpu);
				else
				{
					procesosActuales[cpu] = colasxCore[cpu].front();
					colasxCore[cpu].pop();
					return procesosActuales[cpu].pid;
				}

			}
			//No se me termino el quantum, sigo corriendo
			return procesosActuales[cpu].pid;
		}
	}
	return 0;
}