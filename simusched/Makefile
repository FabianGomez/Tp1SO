export LC_ALL=C
CXXFLAGS= -Wall -pedantic -g -ggdb
LDFLAGS= -lpthread

OBJS=main.o simu.o basesched.o basetask.o tasks.o sched_rr.o sched_fcfs.o sched_rr2.o sched_no_mistery.o
MAIN=simusched

.PHONY: all clean new
all: $(MAIN)

$(MAIN): $(OBJS) sched_mistery.o 
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	make ejercicios

%.o: %.cpp %.h basesched.h
basesched.o: basesched.cpp basesched.h
basetask.o: basetask.cpp basetask.h
main.o: main.cpp simu.h basetask.h basesched.h tasks.h sched_fcfs.h \
 sched_rr.h sched_rr2.h
simu.o: simu.cpp simu.h basetask.h basesched.h
simusched.o: simusched.cpp basetask.h basesched.h tasks.h
tasks.o: tasks.cpp tasks.h basetask.h

clean:
	rm -f $(OBJS) $(MAIN)

ejercicio1:
	./simusched lotes/TaskEjercicio1.tsk 1 1 0 SchedFCFS | python graphsched.py > ../res/ej_1_test.png
ejercicio2: 
	./simusched lotes/TaskEjercicio2.tsk 1 4 0 SchedFCFS | python graphsched.py > ../res/ej_2_test_1core.png
	./simusched lotes/TaskEjercicio2.tsk 2 4 0 SchedFCFS | python graphsched.py > ../res/ej_2_test_2cores.png
ejercicio3: 
	./simusched lotes/TaskEjercicio3.tsk 1 1 0 SchedFCFS | python graphsched.py > ../res/ej_3_test.png
ejercicio4:
	./simusched lotes/testej4.tsk 2 1 0 SchedRR 3 3 | python graphsched.py > ../res/ej_4_test_2c_3t.png
	./simusched lotes/TaskEjercicio4.tsk 1 1 0 SchedRR 5| python graphsched.py > ../res/TaskEjercicio4.png
ejercicio5: 
	./simusched lotes/TaskEjercicio5.tsk 1 2 0 SchedRR 10 | python graphsched.py > ../res/ej_5_test_10.png
	./simusched lotes/TaskEjercicio5.tsk 1 2 0 SchedRR 2 | python graphsched.py > ../res/ej_5_test_2.png
	./simusched lotes/TaskEjercicio5.tsk 1 2 0 SchedRR 50 | python graphsched.py > ../res/ej_5_test_50.png
ejercicio6: 
	./simusched lotes/TaskEjercicio5.tsk 1 2 0 SchedFCFS | python graphsched.py > ../res/ej_6_test_FCFA.png
ejercicio7:
	./simusched lotes/tasktestej7.tsk 1 1 0 SchedMistery 2 5 7 8 15 2 4 5 | python graphsched.py > ../res/ejercicio7_1.png
	./simusched lotes/tasktestej7.tsk 1 1 0 SchedMistery 10 | python graphsched.py > ../res/ejercicio7_2.png
	./simusched lotes/taskHardcodeada1.tsk 1 1 0 SchedMistery 5 3 2 13 7 4| python graphsched.py > ../res/ejercicio7_3.png
	./simusched lotes/TaskEjercicio7.tsk 1 1 0 SchedNoMistery 10 20 30 40 | python graphsched.py > ../res/TaskEjercicio7Nuestro.png
	./simusched lotes/TaskEjercicio7.tsk 1 1 0 SchedMistery 10 20 30 40 | python graphsched.py > ../res/TaskEjercicio7.png

ejercicio8:
	./simusched lotes/TaskEjercicio8_migracion.tsk 2 1 10 SchedRR2 5 5 | python graphsched.py > ../res/Ejercicio8_migracion_1.png
	./simusched lotes/TaskEjercicio8_migracion.tsk 2 1 10 SchedRR 5 5 | python graphsched.py > ../res/Ejercicio8_migracion_2.png
	./simusched lotes/TaskEjercicio8_sinmigracion.tsk 2 1 0 SchedRR2 10 10 | python graphsched.py > ../res/ejercicio8_sinmigracion1.png
	./simusched lotes/TaskEjercicio8_sinmigracion.tsk 2 1 30 SchedRR 10 10 | python graphsched.py > ../res/ejercicio8_sinmigracion2.png
ejercicios: ejercicio1 ejercicio2 ejercicio3 ejercicio4 ejercicio5 ejercicio6 ejercicio7 ejercicio8

new: clean all

