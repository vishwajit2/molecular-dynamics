mds: drawTerminal.o Event.o Particle.o Pqueue.o CollisionSystem.o
	gcc -o mds -Wall drawTerminal.o Event.o Particle.o Pqueue.o CollisionSystem.o

CollisionSystem.o: CollisionSystem.c Particle.h
	gcc -c CollisionSystem.c 

drawTerminal.o: drawTerminal.c Particle.h
	gcc -c drawTerminal.c 

Particle.o: Particle.c Particle.h
	gcc -c Particle.c 

Pqueue.o: Pqueue.c Pqueue.h
	gcc -c Pqueue.c 

Event.o: Event.c Event.h
	gcc -c Event.c 
