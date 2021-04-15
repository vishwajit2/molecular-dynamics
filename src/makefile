mds: drawTerminal.o Event.o Particle.o PQueue.o CollisionSystem.o utilities.o fileIO.o Simulation.o drawGL.o
	gcc -o mds -Wall drawTerminal.o Event.o Particle.o PQueue.o CollisionSystem.o fileIO.o utilities.o Simulation.o drawGL.o -lGL -lGLU -lglut -lm 

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

fileIO.o: fileIO.c fileIO.h
	gcc -c fileIO.c

Simulation.o: Simulation.c Simulation.h
	gcc -c Simulation.c

drawGL.o: drawGL.c
	gcc -c drawGL.c

debug: drawTerminal.c Event.c Particle.c PQueue.c CollisionSystem.c fileIO.c Simulation.c drawGL.c
	gcc -o mdsDebug drawGL.c Particle.c Event.c PQueue.c CollisionSystem.c drawTerminal.c utilities.c fileIO.c Simulation.c -lGL -lGLU -lglut -lm -g