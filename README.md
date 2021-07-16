# molecular-dynamics
System to simulate and analyze the motion of N colliding particles according to   
the laws of elastic collision using event-driven simulation.

You can watch demo by cloning the repo and executing ./demo.sh

### Functionality
+ simulate the collision system (using openGL or on terminal)  
+ predict system state after a given a time  
+ calculate thermodynamic properties of system  
+ keep record of selected particles  
+ create a random collision system efficiently  
+ read system stae from and save results to file  
+ draw trails of specific particles as they move

### Usage
./mds -f filename limit calculateSP   
./mds -r n limit calculateSP customEnable

- limit : system time for which we simulate (optional, default = 5000)  
- filename : file to read input from (with -r flag)  
- calculateSP : 1 for calculating properties (optional, default = 0)  
- if custom is 1, we'll ask you for particle configuration

### TODO and possible improvements 
• Implement cell method to reduce computation\
• Parallelization of the algorithm to make use of all available CPU cores\
• Extend this 2D engine to n- dimensions. Analyze collision of n\
• Handle multi-particle collisions dimensional hyperplanes

A good reference to learn more about Event Driven Molecular Dynamics Simulations https://arxiv.org/ftp/physics/papers/0405/0405089.pdf
