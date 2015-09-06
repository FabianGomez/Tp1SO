#include <vector>
#include <queue>
#include "sched_no_mistery.h"
#include "basesched.h"
#include <iostream>

using namespace std;

SchedNoMistery::SchedNoMistery(vector<int> argn) {  

	/*quantums = vector<int>(argn.size()+ 1,0);
	quantums[0]=1; // por defecto
	for(unsigned int i=1;i<quantums.size();i++){
		quantums[i]=argn[i-1];
	}*/
	quantums = vector<int>(argn);
	quantumActual = quantums[0];
	index=0;
	procesos=0;
	cola.clear();
	//quantumBloqueados.clear();
}

void SchedNoMistery::load(int pid) {  
	pcb p = pcb();
	p.pid = pid;

	cola.push_back(p);
	procesos++;
}

void SchedNoMistery::unblock(int pid) {

	//cout << "ME DESBLOQUEO " << pid << endl;

    pcb p = pcb();
    p.pid = pid; // esta en ready incialmente.

    // gana maxima prioridad.
    cola.push_front(p);

    //cout << "FRONT " << cola.front().pid << endl;

    // aparecio uno nuevo en la ronda.
    procesos = procesos + 2;

    // cuando se bloquee no lo vamos a almacenar
    // sabemos que cuando se desbloquee irrumpe en el orden.
}

void SchedNoMistery::cambiarQuantum(unsigned int pid)
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


}

int SchedNoMistery::tick(int cpu, const enum Motivo m) {  

	// EN COLA NO SON LOS QUE SE ESTAN EJECUTANDO

	if( m == EXIT || m == BLOCK) 
	{
		// si hace exit o se desaloja por estar bloqueado hay que resetear el quantum.

		// reseteamos el quantum

		procesos--;
			// mantenemos el mismo.
				// tenemos que resetear procesos.
		if(procesos == 0)
		{
			procesos = cola.size();
			// se termino una ronda
			index = index < quantums.size() -1 ? index+1 : index;
				
		}

		quantumActual = quantums[index];
		// sobre el que decido.
		//pcb primero = pcb();
		//primero.pid = current_pid(cpu);

		// En este scheduler no es necesario pushearse si se bloqueo
		// Cuando se desbloquee se lo pushea primero ready.

		if (m == BLOCK)
			quantumBloqueados[0] = 0;

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
		quantumActual--; //Si estoy en la idle me puede dar negativo, pero no importa porque lo piso
		
		if (cola.size() == 0 && current_pid(cpu) == IDLE_TASK)
			//No hay nadie para correr, sigo con la idle
			return current_pid(cpu);
		else if(cola.size() > 0 && current_pid(cpu) == IDLE_TASK)
		{
			//Hay alguien en la cola. Tengo que ver si hay alguien ready
		
			procesos = cola.size();
			quantumActual = quantums[index];

			pcb res = cola.front();
			cola.pop_front();

			cambiarQuantum(res.pid);
			return res.pid;
		}
		else if(cola.size() == 0 && current_pid(cpu) != IDLE_TASK)
		{
			//No hay nadie esperando, solo tengo que verificar si hay que resetear el quantum
			//cout << "ANTES DE DESBLOQUEARSE " << quantumActual<<  endl;
			if(quantumActual == 0)
			{
				index = index < quantums.size()-1 ? index+1 : index;
				quantumActual = quantums[index];
				procesos = 1;
			}

			cambiarQuantum(current_pid(cpu));
			return current_pid(cpu);
		}
		else
		{
			//cout << "CAMBIO DE TAREAS " << quantumActual << endl;
			//Hay alguien en la cola y no estoy corriendo la idle.
			if(quantumActual == 0)
			{
				//cout << "ACTUAL ES  " << current_pid(cpu) << endl;

				procesos--;

				index = procesos == 0 && index < quantums.size()-1 ? index+1 : index;

				procesos = procesos == 0 ? cola.size() + 1 : procesos;
				quantumActual = quantums[index];

				pcb res = cola.front();
				cola.pop_front();

				//cout << "RES es " << res.pid << endl;

				pcb actual = pcb();
				actual.pid = current_pid(cpu);
				cola.push_back(actual); 

				cambiarQuantum(res.pid);
				return res.pid;

			}

			//No se me termino el quantum, sigo corriendo
			return current_pid(cpu);
		}
	}
	
	return 0;

}
