
g++ -shared -o libadd.so add.cpp
g++ -shared -o libmul.so mul.cpp
g++ -shared -o libpow.so pow.cpp

g++ main.cpp -L. -lpow  -lmul -ladd -omain

LD_LIBRARY_PATH=. ./main


