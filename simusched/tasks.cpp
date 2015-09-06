#include "tasks.h"
#include <stdlib.h> 
#include <iostream>  
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

void TaskConsola(int pid,vector<int> params){
	int n = params[0];
	int min = params[1];
	int max = params[2];

	for(int i=0;i<n;i++){
		//int tiempo = rand() % max + min;
		int tiempo = rand() % (max-min+1) + min;
		uso_IO(pid,tiempo);//No estoy contando el tiempo de paso a bloqueo
	}
}

void TaskBatch(int pid,vector<int> params){

	int total_cpu=params[0];
	int cant_bloqueos=params[1];
	if(total_cpu < cant_bloqueos -1)
	{
		cout << "Error" << endl;
		return;
	}

	vector<bool> tiempos(total_cpu, false);
	int i = 0; 
	while(i < cant_bloqueos)
	{
		int idx = rand() % (total_cpu -1); //para que no pueda caer uno en el tiempo del exit
		if(tiempos[idx] == true)
			continue;
		else
		{
			tiempos[idx] = true;
			i++;
		}
	}


	for(i = 0; i < total_cpu; i++)
	{
		if(tiempos[i] == true)
			uso_IO(pid, 1);
		else
			uso_CPU(pid, 1);
	}
}

void TaskHarcodeada(int pid,vector<int> params){
	uso_CPU(pid, 10);
	uso_IO(pid, 2);
	uso_CPU(pid, 8);
	//uso_IO(pid, 5);
	uso_CPU(pid, 100);
	//uso_IO(pid, 9)
}
void TMaunsi(int pid,vector<int> params)
{
	uso_CPU(pid, 10);
	uso_IO(pid, 5);
	uso_CPU(pid, 10);
	uso_IO(pid, 5);
	uso_CPU(pid, 10);
	uso_IO(pid, 5);
	uso_CPU(pid, 10);
	uso_IO(pid, 5);
	uso_CPU(pid, 10);
	uso_CPU(pid, 10);
	uso_CPU(pid, 10);
}
void TaskHarcodeada1(int pid,vector<int> params){
	uso_CPU(pid, 25);
	uso_IO(pid, 20);
	uso_CPU(pid, 25);
}



void tasks_init(void) {
	/* Todos los tipos de tareas se deben registrar acá para poder ser usadas.
	 * El segundo parámetro indica la cantidad de parámetros que recibe la tarea
	 * como un vector de enteros, o -1 para una cantidad de parámetros variable. */
	register_task(TaskCPU, 1);
	register_task(TaskIO, 2);
	register_task(TaskAlterno, -1);
	register_task(TaskConsola, 3);
	register_task(TaskBatch, 2);
	register_task(TaskHarcodeada, 0);
	register_task(TaskHarcodeada1, 0);
	register_task(TMaunsi, 0);
}