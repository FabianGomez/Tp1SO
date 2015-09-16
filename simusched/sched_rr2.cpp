#include <vector>
#include <queue>
#include "sched_rr2.h"
#include "basesched.h"
#include <iostream>
#include <map>

using namespace std;

SchedRR2::SchedRR2(vector<int> argn) {
	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	unsigned int n = argn[0];
	quantums = vector<int>(n,0);
	quantumsActuales = vector<int>(n,0);
	colasxCore = vector< std::queue<pcb> >(n);
	for(unsigned int i =1;i<n+1;i++){
		quantums[i-1] = argn[i];
		quantumsActuales[i-1] = argn[i];
		colasxCore[i-1] = std::queue<pcb>();
	}


}

SchedRR2::~SchedRR2() {

}


void SchedRR2::load(int pid) {

	unsigned int running= current_pid(0) != -1 ? 1 : 0;
	unsigned int min=colasxCore[0].size() + running;
	unsigned int minindex=0;

	for(unsigned int i=1;i<colasxCore.size();i++){ 
		running= current_pid(i) != -1 ? 1 : 0;
	
		if(colasxCore[i].size() + running < min){
			min=colasxCore[i].size() + running;
			minindex=i;
		}
	}
	pcb p = pcb();
	p.pid = pid;
	colasxCore[minindex].push(p);
}

void SchedRR2::unblock(int pid)
{
	pcb bloqueado;
	int indice = -1;
	for(unsigned int i = 0; i < colasxCore.size(); i++)
	{
		
		for(unsigned int j = 0; j < colasxCore[i].size(); j++)
		{
			pcb p = colasxCore[i].front();
			colasxCore[i].pop();
			if(p.pid == pid)
			{
				bloqueado=p;
				indice = i;
			}
			else
				colasxCore[i].push(p);
		}

		if (indice != -1)
		{
			bloqueado.estado=Ready;
			colasxCore[indice].push(bloqueado);
			break;
		}
		
	}
}


int SchedRR2::tick(int cpu, const enum Motivo m) {
	// EN COLA NO SON LOS QUE SE ESTAN EJECUTANDO
	// PARA ESO ESTA procesosActuales!!!!!

	if( m == EXIT || m == BLOCK) 
	{
		// reseteamos el quantum
		quantumsActuales[cpu] = quantums[cpu];

		// sobre el que decido.
		pcb primero = pcb();
		primero.pid = current_pid(cpu);

		// se contempla tambien el caso vacio
		unsigned int i = 0;
		while(colasxCore[cpu].front().estado == Blocked && i < colasxCore[cpu].size())
		{
			pcb p = colasxCore[cpu].front();
			colasxCore[cpu].pop();
			colasxCore[cpu].push(p);
			i++;
		}

		

		if(i == colasxCore[cpu].size()){
			// me vuelvo a pushear, si me bloqueé
			if (m == BLOCK){
				primero.estado = Blocked;
				colasxCore[cpu].push(primero);
			}
			return IDLE_TASK;
		}
		else
		{
			// me vuelvo a pushear, si me bloqueé
			if (m == BLOCK){
				primero.estado = Blocked;
				colasxCore[cpu].push(primero);
			}
			pcb res = colasxCore[cpu].front();
			colasxCore[cpu].pop();
			return res.pid;
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
			unsigned int i = 0;
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
				pcb res = colasxCore[cpu].front();
				colasxCore[cpu].pop();
				return res.pid;
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
				unsigned int i = 0;
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
					pcb actual = pcb();
					actual.pid = current_pid(cpu);
					colasxCore[cpu].push(actual); 
					pcb res = colasxCore[cpu].front();
					colasxCore[cpu].pop();
					return res.pid;
				}

			}
			//No se me termino el quantum, sigo corriendo
			return current_pid(cpu);
		}
	}
	
	return 0;
}