g++ -c add.cpp
g++ -c mul.cpp
g++ -c pow.cpp
ar rcs libadd.a add.o
ar rcs libmul.a mul.o
ar rcs libpow.a pow.o
