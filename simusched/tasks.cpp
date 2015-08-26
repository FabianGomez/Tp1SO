#include "tasks.h"
#include <stdlib.h>   
//rand() % max + min; Random entre min y max
using namespace std;

void TaskCPU(int pid, vector<int> params) { // params: n
	uso_CPU(pid, params[0]); // Uso el CPU n milisegundos.
}

void TaskIO(int pid, vector<int> params) { // params: ms_pid, ms_io,
	uso_CPU(pid, params[0]); // Uso el CPU ms_pid milisegundos.
	uso_IO(pid, params[1]); // Uso IO ms_io milisegundos.
}

void TaskAlterno(int pid, vector<int> params) { // params: ms_pid, ms_io, ms_pid, ...
	for(int i = 0; i < (int)params.size(); i++) {
		if (i % 2 == 0) uso_CPU(pid, params[i]);
		else uso_IO(pid, params[i]);
	}
}

void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
}


void TaskConsola(int pid,vector<int> params){
	int n = params[0];
	int min = params[1];
	int max = params[2];

	for(int i=0;i<n;i++){
		int tiempo = rand() % max + min;
		uso_IO(pid,tiempo);//No estoy contando el tiempo de paso a bloqueo
	}
}

void TaskBatch(int pid,vector<int> params){
	int total_cpu=params[0];
	int cant_bloqueos=params[1];
	int rango_temp=total_cpu % cant_bloqueos;

	for(int i=0;i<cant_bloqueos;i++){
		int tiempo = rand() % (rango_temp-1);
		uso_IO(pid, 1);
		uso_CPU(pid,tiempo);
		total_cpu = total_cpu - tiempo;
	}
	if(total_cpu > 0)
		uso_CPU(pid,total_cpu);
}