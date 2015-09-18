
#include "sched_rr.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedRR::SchedRR(vector<int> argn) {

	// Round robin recibe la cantidad de cores y sus cpu_quantum por parámetro
	int n = argn[0];
	quantums = vector<int>(n,0);
	quantumsActuales = vector<int>(n,0);
	for(int i =1;i<n+1;i++){
		quantums[i-1] = argn[i];
		quantumsActuales[i-1] = argn[i];
	}

	

}

SchedRR::~SchedRR() {

}


void SchedRR::load(int pid) 
{
	//creamos una nueva pcb para la tarea
	pcb p = pcb();
	p.pid = pid;
	//la encolamos
	cola.push(p);
}

void SchedRR::unblock(int pid) 
{
	//recorremos la cola hasta dar una vuelta completa, para no alterar el orden de los procesos, y encolamos el desbloqueado atras de todo
	pcb bloqueado;
	int size = cola.size();
	for(unsigned int i=0; i < size;i++)
		{
			pcb p = cola.front();
			//como se desbloqueo le cambiamos el estado
			cola.pop();
			
			if(p.pid == pid)
				bloqueado=p;
			else
				cola.push(p);
		}
	//la cola queda exactamente igual
	bloqueado.estado=Ready;
	cola.push(bloqueado);
}

unsigned int SchedRR::siguiente()
{
	unsigned int i = 0;
	while(cola.front().estado == Blocked && i < cola.size())
	{
		pcb p = cola.front();
		cola.pop();
		cola.push(p);
		i++;
	}
	return i;
}
int SchedRR::tick(int cpu, const enum Motivo m) {

	if( m == EXIT || m == BLOCK) 
	{
		// reseteamos el quantum para que el proximo proceso que corra lo pueda usar todo
		quantumsActuales[cpu] = quantums[cpu];

		// creo una pcb para la tarea que me llamo
		pcb primero = pcb();
		primero.pid = current_pid(cpu);

		// se contempla tambien el caso vacio
		
		// recorro la cola hasta encontrar un proceso bloqueado, o hasta que la haya recorrida toda
		unsigned int i = siguiente();
		if(i == cola.size()){
			// no encontre un proceso que estuviera Ready
			// me vuelvo a pushear, si me bloqueé
			if (m == BLOCK){
				primero.estado = Blocked;
				cola.push(primero);
			}
			return IDLE_TASK;
		}
		else
		{
			// encontre un proceso Ready
			// me vuelvo a pushear, si me bloqueé
			if (m == BLOCK){
				primero.estado = Blocked;
				cola.push(primero);
			}
			pcb res = cola.front();
			cola.pop();
			return res.pid;
		}

	} else if (m == TICK)
	{
		//trascurrio un tick, entonces decremento quantumsActuales
		quantumsActuales[cpu]--; //Si estoy en la idle me puede dar negativo, pero no importa porque lo piso
		if (cola.size() == 0 && current_pid(cpu) == IDLE_TASK)
			//No hay nadie para correr, sigo con la idle
			return current_pid(cpu);
		else if(cola.size() > 0 && current_pid(cpu) == IDLE_TASK)
		{
			//Hay alguien en la cola. Tengo que ver si hay alguien ready
			unsigned int i = siguiente();
			if(i == cola.size())
				//No hay nadie ready, le toca al proceso actual de nuevo
				return current_pid(cpu);
			else
			{
				quantumsActuales[cpu] = quantums[cpu];
				pcb res = cola.front();
				cola.pop();
				return res.pid;
			}
		}
		else if(cola.size() == 0 && current_pid(cpu) != IDLE_TASK)
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
				unsigned int i = siguiente();
				quantumsActuales[cpu] = quantums[cpu];
				if(i == cola.size())
					//No hay nadie mas ready, le toca al proceso actual de nuevo
					return current_pid(cpu);
				else
				{
					pcb actual = pcb();
					actual.pid = current_pid(cpu);
					cola.push(actual); 
					pcb res = cola.front();
					cola.pop();
					return res.pid;
				}

			}
			//No se me termino el quantum, sigo corriendo
			return current_pid(cpu);
		}
	}
	// no deberia llegar nunca aca
	return IDLE_TASK;
}

