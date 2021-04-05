mds: drawTerminal.o Event.o Particle.o PQueue.o CollisionSystem.o utilities.o
	gcc -o mds -Wall drawTerminal.o Event.o Particle.o PQueue.o CollisionSystem.o utilities.o -lm

CollisionSystem.o: CollisionSystem.c Particle.h
	gcc -c CollisionSystem.c 

drawTerminal.o: drawTerminal.c Particle.h
	gcc -c  drawTerminal.c 

Particle.o: Particle.c Particle.h
	gcc -c Particle.c -lm

PQueue.o: PQueue.c PQueue.h
	gcc -c PQueue.c 

Event.o: Event.c Event.h
	gcc -c Event.c 

utilities.o: utilities.c utilities.h
	gcc -c utilities.c

debug: drawTerminal.c Event.c Particle.c PQueue.c CollisionSystem.c
	gcc -o mdsDebug  -g  -Wall drawTerminal.c Event.c Particle.c PQueue.c CollisionSystem.c -lm