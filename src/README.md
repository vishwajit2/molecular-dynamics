# molecular-dynamics

watch demo by running ./demo.sh

execute from command line with
./mds -f filename limit calculateSP
./mds -r n limit calculateSP customEnable

limit : system time for which we simulate // optional, default = 5000
filename : file to read input from (with -r flag)
calculateSP : 1 for calculating properties // optional, default = 0
if custom is 1, we'll ask you for particle configuration // optional
