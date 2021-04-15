#include <stdio.h>
#include <stdlib.h>

#include "Simulation.h"

int main(int argc, char *argv[])
{
    double limit = 5000;
    int calculateSP = 0;
    char *filename;
    int n;
    int custom;
    /**
    * ./mds -f filename limit calculateSP
    * ./mds -r n limit calculateSP customEnable
    * 
    * limit : system time for which we simulate // optional, default = 5000
    * filename : file to read input from (with -r flag) 
    * calculateSP : 1 for calculating properties // optional, default = 0 
    * if custom is 1, we'll ask you for particle configuration // optional
    * */
    int i = 0;
    if (argc >= 3)
    {
        if (argv[1][0] == '-' && argv[1][1] == 'f')
        {
            filename = argv[2];
            if (argc == 4)
                limit = atof(argv[3]);
            else if (argc >= 5)
            {
                limit = atof(argv[3]);
                calculateSP = atoi(argv[4]);
            }

            startSimulation(argc, argv, limit, filename, 0, calculateSP, NULL);
        }
        else if (argv[1][0] == '-' && argv[1][1] == 'r')
        {
            n = atoi(argv[2]);
            if (argc >= 4)
                limit = atof(argv[3]);
            if (argc >= 5)
            {
                calculateSP = atoi(argv[4]);
            }
            if (argc >= 6)
            {
                custom = atoi(argv[5]);
            }
            if (custom == 1)
            {
                ParticleConfig conf = getParticleConfig();
                printf("Enter mass of  particle\n");
                scanf("%lf", &conf.mass);
                printf("Enter radius of  particle\n");
                scanf("%lf", &conf.radius);
                printf("Enter velocity range of  particles : lower bound\n");
                scanf("%lf", &conf.vel_lo);
                printf("Enter velocity range of  particles : upper bound\n");
                scanf("%lf", &conf.vel_hi);
                startSimulation(argc, argv, limit, NULL, n, calculateSP, &conf);
            }
            else
            {
                startSimulation(argc, argv, limit, NULL, n, calculateSP, NULL);
            }
        }
        else
        {
            printf("Enter proper flags \n");
        }
    }
    else
    {
        // start default random simulation with n = 200;
        n = 200;
        startSimulation(argc, argv, limit, NULL, n, calculateSP, NULL);
    }

    return 0;
}
