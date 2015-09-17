#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {  

	quantums = vector<int>(argn);
	
	indices = vector<int>();
	quantumsActuales = vector<int>();

	cola.clear();
}

void SchedNoMistery::load(int pid) {  
	//creo una nueva pcb para el proceso
	pcb p = pcb();
	p.pid = pid;
	//lo encolo
	// FIJARSE QUE PUEDE SER PUSH FRONT.
	cola.push_back(p);

	// le asignamos
	indices.push_back(0);
	quantumsActuales.push_back(quantums[0]);
}

void SchedNoMistery::unblock(int pid) 
{
    pcb p = pcb();
    p.pid = pid; // esta en ready incialmente.

    //cola.push_back(p);


    // gana maxima prioridad.
    cola.push_front(p);

    indices[pid] = 0;
    quantumsActuales[pid] = quantums[0];
}


// Obtiene el valor del quantum especial que toma el proceso al ejecutarse luego de un bloqueo
/*void SchedNoMistery::cambiarQuantum(unsigned int pid)
{
	std::map<unsigned int, unsigned int>::iterator it;

  	it = quantumBloqueados.find(pid);
  	if (it != quantumBloqueados.end()) // lo encontre
  	{
  		cout << "ENTRE A CAMBIAR QUANTUM" << endl;
  		unsigned int suma = 0;
  		for (unsigned int i = quantumBloqueados[pid]; i < index; i++)
  			suma += quantums[i];

  		quantumActual = suma;

  		quantumBloqueados.erase(it);
  	}


}*/


int SchedNoMistery::tick(int cpu, const enum Motivo m) {  

	// EN COLA NO SON LOS QUE SE ESTAN EJECUTANDO

	if( m == EXIT || m == BLOCK) 
	{
		// si hace exit o se desaloja por estar bloqueado hay que resetear el quantum.


		// En este scheduler no es necesario pushearse si se bloqueo
		// Cuando se desbloquee se lo pushea primero ready.

		if(0 == cola.size()){
			return IDLE_TASK;
		}
		else
		{
			pcb res = cola.front();
			cola.pop_front();
			return res.pid;
		}

	} else if (m == TICK)
	{
		if (current_pid(cpu) != -1)
			quantumsActuales[current_pid(cpu)]--;
		
		if (cola.size() == 0 && current_pid(cpu) == IDLE_TASK)
			//No hay nadie para correr, sigo con la idle
			return current_pid(cpu);
		else if(cola.size() > 0 && current_pid(cpu) == IDLE_TASK)
		{
			//Hay alguien en la cola. 
		
			pcb res = cola.front();
			cola.pop_front();

			return res.pid;
		}
		else if(cola.size() == 0 && current_pid(cpu) != IDLE_TASK)
		{
			int pid = current_pid(cpu);
			//No hay nadie esperando, solo tengo que verificar si hay que resetear el quantum
			if(quantumsActuales[pid] == 0)
			{
				indices[pid] = indices[pid] < quantums.size()-1 ? indices[pid]+1 : indices[pid];
				quantumsActuales[pid] = quantums[indices[pid]];
			}

			return pid;
		}
		else
		{
			int pid = current_pid(cpu);
			//Hay alguien en la cola y no estoy corriendo la idle.
			if(quantumsActuales[pid] == 0)
			{
				indices[pid] = indices[pid] < quantums.size()-1 ? indices[pid]+1 : indices[pid];
				quantumsActuales[pid] = quantums[indices[pid]];

				pcb res = cola.front();
				cola.pop_front();

				// me busco si estoy pusheado.

				/*bool estoy = false;
				for(unsigned int i=0; i < cola.size();i++)
				{
					pcb p = cola.front();
					if (!estoy)
						estoy = p.pid == current_pid(cpu);
					cola.pop();
					cola.push(p);
				}*/

				//if (!estoy){
					pcb actual = pcb();
					actual.pid = current_pid(cpu);
					cola.push_back(actual); 
				//}
				return res.pid;

			}

			//No se me termino el quantum, sigo corriendo
			return current_pid(cpu);
		}
	}
	return 0;

}

