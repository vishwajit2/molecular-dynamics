# run this to see demo
# give permission by executing :  chmod +x demo.sh
cd src
make
./mds -f input/diffusion.txt
./mds -f input/brownian.txt
./mds -f input/p10.txt
./mds -f input/p100-.5K.txt
./mds -r 300